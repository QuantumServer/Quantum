#include "utils/TextFormat.h"

#ifdef WIN32

const std::string TextFormat::BLACK = "";
const std::string TextFormat::DARK_BLUE = "";
const std::string TextFormat::DARK_GREEN = "";
const std::string TextFormat::DARK_AQUA = "";
const std::string TextFormat::DARK_RED = "";
const std::string TextFormat::PURPLE = "";
const std::string TextFormat::GOLD = "";
const std::string TextFormat::GRAY = "";
const std::string TextFormat::DARK_GRAY = "";
const std::string TextFormat::BLUE = "";
const std::string TextFormat::GREEN = "";
const std::string TextFormat::AQUA = "";
const std::string TextFormat::RED = "";
const std::string TextFormat::LIGHT_PURPLE = "";
const std::string TextFormat::YELLOW = "";
const std::string TextFormat::WHITE = "";

const std::string TextFormat::BOLD = "";
const std::string TextFormat::OBFUSCATED = "";
const std::string TextFormat::ITALIC = "";
const std::string TextFormat::UNDERLINE = "";
const std::string TextFormat::STRIKETHROUGH = "";
const std::string TextFormat::RESET = "";

#else

const std::string TextFormat::BLACK = "\x1b[38;5;16m";
const std::string TextFormat::DARK_BLUE = "\x1b[38;5;19m";
const std::string TextFormat::DARK_GREEN = "\x1b[38;5;34m";
const std::string TextFormat::DARK_AQUA = "\x1b[38;5;37m";
const std::string TextFormat::DARK_RED = "\x1b[38;5;124m";
const std::string TextFormat::PURPLE = "\x1b[38;5;127m";
const std::string TextFormat::GOLD = "\x1b[38;5;214m";
const std::string TextFormat::GRAY = "\x1b[38;5;145m";
const std::string TextFormat::DARK_GRAY = "\x1b[38;5;59m";
const std::string TextFormat::BLUE = "\x1b[38;5;63m";
const std::string TextFormat::GREEN = "\x1b[38;5;83m";
const std::string TextFormat::AQUA = "\x1b[38;5;87m";
const std::string TextFormat::RED = "\x1b[38;5;203m";
const std::string TextFormat::LIGHT_PURPLE = "\x1b[38;5;207m";
const std::string TextFormat::YELLOW = "\x1b[38;5;227m";
const std::string TextFormat::WHITE = "\x1b[38;5;231m";

const std::string TextFormat::BOLD = "\x1b[1m";
const std::string TextFormat::OBFUSCATED = "";
const std::string TextFormat::ITALIC = "\x1b[3m";
const std::string TextFormat::UNDERLINE = "\x1b[4m";
const std::string TextFormat::STRIKETHROUGH = "\x1b[9m";
const std::string TextFormat::RESET = "\x1b[m";

#endif
