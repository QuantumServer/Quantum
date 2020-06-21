#include "config.h"

#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include <boost/fiber/algo/asio.hpp>
#include <boost/fiber/asio/use_future.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/locale/format.hpp>
#include <boost/locale/generator.hpp>
#include <boost/locale/message.hpp>
#include <boost/program_options.hpp>
#include "server/servermanager.h"

int main(int argc, char *argv[]) {

    boost::locale::generator localeGen;
    localeGen.add_messages_path(PACKAGE_LOCALE_DIR);
    localeGen.set_default_messages_domain(GETTEXT_PACKAGE);
    boost::program_options::options_description desc;
    std::string configFile;

    desc.add_options()(
        "help,h",
        boost::locale::translate("display this help and exit").str().c_str()
    )(
        "version,V",
        boost::locale::translate("output version information and exit")
        .str()
        .c_str()
    )(
        "config,c",
        boost::program_options::value<std::string>(&configFile)
            ->value_name(boost::locale::translate("configuration file"))
            ->default_value(CONFIG_DIR "/cenisys.conf"),
            boost::locale::translate("set the configuration file").str().c_str()
    );

    boost::program_options::variables_map vm;

    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm
    );

    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc;
        return 0;
    }

    if (vm.count("version")) {
        std::locale oldCoutLoc = std::cout.imbue(localeGen(""));
        std::cout
            << boost::locale::format(
                boost::locale::translate("Cenisys {1}\n")
            ) % SERVER_VERSION
        std::cout.imbue(oldCoutLoc);
        return 0;
	}

    Cenisys::ServerManager manager(configFile);
    return manager.exec();

}
