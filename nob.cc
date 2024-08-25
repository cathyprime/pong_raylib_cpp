#include "build.hh"
using namespace Kitchen;

int main(int argc, char** argv)
{
	GO_REBUILD_YOURSELF(argc, argv);
	Sink::stage(1);

	std::filesystem::path executable_path = Kitchen::Sink::shift_args(&argc, &argv);
	std::string executable_str = executable_path.filename().string();
	std::string exec_stage2 = "bin/" + executable_str + "_stage2";

	Ingredients files;
	files += "./src/build/" + executable_str + "_stage2.cc";

	CompilerRecipe stage2 = CompilerRecipe()
								.compiler(CC)
								.std_version("c++23")
								.optimization(Heat::Oz)
								.output(exec_stage2)
								.files(files)
								.cache();

	EXEC_EXIT_NON_ZERO(Kitchen::cook(&stage2), "failed to compile the stage 2 binary");

	std::vector<std::string> command(argv, argv + argc);
	command.insert(command.begin(), exec_stage2);
	DESSERT(command);
}
