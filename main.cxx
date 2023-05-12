#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <cassert>

enum class TokenKind { Number, Operator };
enum class Operator { Plus };

struct token_t {
  TokenKind kind;
  Operator op;
  int64_t num;
};

enum class ParseStateKind { Start, Error, Number, Operator };
const char* psk_to_string(ParseStateKind p) {
  const char* names[] = {"Start", "Error", "Number", "Operator"};
  return names[static_cast<int>(p)];
}

struct parse_state_t {
  ParseStateKind fsm_state;
  int64_t num;
};

std::string parse_state_to_string(parse_state_t p) {
  return std::string(psk_to_string(p.fsm_state)) + " " + std::to_string(p.num);
}

enum class CharacterKind { Digit, Operator, Space, Other };

CharacterKind classify_char(unsigned char c) {
  if (std::isdigit(c)) {
    return CharacterKind::Digit;

  } else if (c == '+') {
    return CharacterKind::Operator;

  } else if (c == ' ') {
    return CharacterKind::Space;
  }  else {
    return CharacterKind::Other;
  }
}

token_t plus_token() {
  return token_t {
    TokenKind::Operator,
    Operator::Plus,
    0
  };
}

token_t number_token(int n) {
  return token_t {
    TokenKind::Number,
    Operator::Plus,
    n
  };
}

int c2i(unsigned char c) {
  return int(c) - int('0');
}

parse_state_t advance(parse_state_t current, unsigned char c, std::vector<token_t> &tokens) {
  auto answer = current;
  auto ckind = classify_char(c);

  switch(current.fsm_state) {
  case ParseStateKind::Error:
    answer.fsm_state =  ParseStateKind::Error;
    break;

  case ParseStateKind::Operator:
    switch(ckind) {
    case CharacterKind::Operator:
      answer.fsm_state =  ParseStateKind::Error;
      break;

    case CharacterKind::Digit:
      tokens.push_back(plus_token());
      answer.fsm_state =  ParseStateKind::Number;
      answer.num = c2i(c);
      break;

    case CharacterKind::Space:
      tokens.push_back(plus_token());
      answer.fsm_state =  ParseStateKind::Start;
      break;

    case CharacterKind::Other:
      answer.fsm_state =  ParseStateKind::Error;
      break;
    }
    break;

  case ParseStateKind::Number:
    switch(ckind) {
    case CharacterKind::Operator:
      tokens.push_back(number_token(answer.num));

      answer.fsm_state =  ParseStateKind::Operator;
      answer.num = 0;
      break;

    case CharacterKind::Digit:
      answer.fsm_state =  ParseStateKind::Number;
      answer.num = answer.num*10 + c2i(c);
      break;

    case CharacterKind::Space:
      tokens.push_back(number_token(answer.num));

      answer.fsm_state =  ParseStateKind::Start;
      answer.num = 0;
      break;

    case CharacterKind::Other:
      answer.fsm_state =  ParseStateKind::Error;
      break;
    }
    break;

  case ParseStateKind::Start:
    switch(ckind) {
    case CharacterKind::Operator:
      answer.fsm_state =  ParseStateKind::Operator;
      break;

    case CharacterKind::Digit:
      answer.fsm_state =  ParseStateKind::Number;
      answer.num = c2i(c);
      break;

    case CharacterKind::Space:
      answer.fsm_state =  ParseStateKind::Start;
      break;

    case CharacterKind::Other:
      answer.fsm_state =  ParseStateKind::Error;
      break;
    }
    break;
  }

  return answer;
}

enum class ParseResultKind { Success, Failure };
struct parse_result_t {
  ParseResultKind status;
  std::vector<token_t> tokens;
};

parse_result_t parse(std::istream &input) {
  std::vector<token_t> tokens;
  int c = 0;

  auto parse_state = parse_state_t {ParseStateKind::Start, 0};

  while (!input.eof()) {
    c = input.get();
    if(c == EOF) {
      break;
    }

    unsigned char cchar = static_cast<unsigned char>(c);

    if(cchar == '\n') {
      break;
    }

    parse_state = advance(parse_state, cchar, tokens);
    //std::cout << parse_state_to_string(parse_state) << "\n";
  }

  advance(parse_state, ' ', tokens);

  if (parse_state.fsm_state == ParseStateKind::Error) {
    return parse_result_t {
      ParseResultKind::Failure,
      tokens
    };
  } else {
    return parse_result_t {
      ParseResultKind::Success,
      tokens
    };
  }
}


enum class EvalResultKind { Success, Failure };
struct eval_result_t {
  EvalResultKind status;
  int64_t num;
};

eval_result_t evaluate(const std::vector<token_t> &tokens) {
  std::vector<int64_t> stack;

  for(const auto &token : tokens) {
    switch(token.kind) {
    case TokenKind::Number:
      stack.push_back(token.num);
      break;

    case TokenKind::Operator:
      if (stack.size() >= 2) {
        auto fst = stack.back();
        stack.pop_back();

        auto snd = stack.back();
        stack.pop_back();

        if (fst == 191) {
          assert(false);
        }
        stack.push_back(fst+snd);
      } else {
        return eval_result_t {
          EvalResultKind::Failure,
          0
        };
      }
      break;
    }

    if (stack.size() == 13) {
      assert(false);
    }
  }

  if (stack.size() == 1) {
    int64_t num = stack[0];

    if (num == 3) {
      assert(false);
    }

    if (num == 42) {
      assert(false);
    }

    return eval_result_t {
      EvalResultKind::Success,
      num
    };
  } else {
    return eval_result_t {
      EvalResultKind::Failure,
      0
    };
  }
}

int main(int argc, char **) {
  if (argc > 1) {
    assert(false);
    std::cerr << "This program takes no arguments.\n";
    return 1;
  }

  auto parse_result = parse(std::cin);

  if (parse_result.status == ParseResultKind::Success) {
    std::cout << "Parsed " << parse_result.tokens.size() << " tokens.\n";

    auto eval_result = evaluate(parse_result.tokens);

    if (eval_result.status == EvalResultKind::Success) {
      std::cout << eval_result.num << "\n";
    } else {
      std::cerr << "Eval error.\n";
      return 3;
    }
  } else {
    std::cerr << "Parse error.\n";
    return 2;
  }
  return 0;
}
