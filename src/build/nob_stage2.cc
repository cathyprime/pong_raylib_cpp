#include "../../build.hh"
#include <filesystem>
#include <ranges>

namespace views = std::ranges::views;
namespace ranges = std::ranges;
namespace fs = std::filesystem;
using namespace Kitchen;

const std::string external = "./external";
const std::string raylib = external + "/raylib-5.0_linux_amd64/";

// clang-format off
MAKE_HELP(
    "available commands:",
    "   -> run - compile and run the app",
    "   -> help - display help",
    "   -> build - compile the app",
    "   -> clean - remove build artefacts"
)
// clang-format on

void run()
{
    Sink::print_command({"./bin/main"});
    DESSERT({"./bin/main"});
}

void build()
{
    std::vector<std::string> non_main_file_names;
    GET_NON_MAIN_FILES(non_main_file_names, "./src", ".cc");

    Ingredients compile_flags;
    compile_flags += "-Wall";
    compile_flags += "-pedantic";

    LineCook line_cook;
    Ingredients o_files;
    auto o_file_flags = compile_flags;
    o_file_flags += "-c";

    if (non_main_file_names.size() > 0) {
        for (const auto& filename : non_main_file_names) {
            auto files = new Ingredients;
            auto o_file = "build/" + filename + ".o";
            files->add_ingredients("src/" + filename + ".cc");
            auto recipe = new CompilerRecipe;
            recipe->compiler(CC)
                .cache()
                .std_version("c++23")
                .push({"-I" + raylib + "include/"})
                .push(o_file_flags.get_ingredients())
                .output(o_file)
                .files(*files);

            o_files += o_file;
            line_cook += recipe;
        }
    }

    EXEC_EXIT_NON_ZERO(line_cook.cook(), "compilation of .o files has failed");
    CompilerRecipe main_recipe;
    auto main_files = std::move(o_files);
    main_files += "src/main.cc";

    main_recipe.compiler(CC)
        .cache()
        .std_version("c++23")
        .push({"-I" + raylib + "include/"})
        .files(main_files)
        .push({"-L" + raylib + "lib/"})
        .push({"-l:libraylib.a", "-lm", "-pthread"})
        .push({"-Wall", "-pedantic", "-Werror"})
        .output("bin/main");

    EXEC_EXIT_NON_ZERO(cook(&main_recipe), "failed compiling the executable");
}

void clean()
{
    std::string filename = __FILE_NAME__;
    filename = filename.substr(0, filename.find_last_of("."));

    if (fs::exists("./bin")) {
        auto build_files = DIR_RANGE("./bin") | views::filter([&](auto&& value) {
                               fs::path path = ((fs::path)value).filename();
                               return path.filename() != filename;
                           })
                         | ranges::views::transform([&](auto&& value) { return (std::string)(fs::path)value; })
                         | std::views::common | ranges::to<std::vector<std::string>>();

        for (const auto& file : build_files) {
            Sink::print_command({"rm", file});
            fs::remove(file);
        }
    }

    if (fs::exists("./build")) {
        for (const auto& file : fs::directory_iterator("./build")) {
            Sink::print_command({"rm", (fs::path)file});
            fs::remove(file);
        }
    }
}

int main(int argc, char** argv)
{
    Sink::stage(2);
    (void)Sink::shift_args(&argc, &argv);

    if (!fs::exists(raylib)) {
        INFO("Downloading raylib...");
        EXEC_EXIT_NON_ZERO(
            Sink::start_job_sync(
                {"wget", "-P", external,
                 "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz"}),
            "trouble downloading raylib");

        EXEC_EXIT_NON_ZERO(
            Sink::start_job_sync({"tar", "xzf", "./external/raylib-5.0_linux_amd64.tar.gz", "-C", external}),
            "trouble unpacking raylib");
    }

    char* c_arg = Sink::shift_args(&argc, &argv);
    std::string arg = c_arg != nullptr ? c_arg : "";

    if (!arg.empty()) {
        if (arg == "help") {
            help();
        } else if (arg == "build") {
            build();
        } else if (arg == "run") {
            build();
            run();
        } else if (arg == "clean") {
            clean();
        } else {
            ERROR("command \"" + arg + "\" not recognized");
            help();
            return 1;
        }
    } else {
        build();
    }
}
