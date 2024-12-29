#include "config.hpp"

int main(int argc, char **argv) {
	(void)argc; (void)argv;

	try {
		GLFWwindow *window = CreateWindow();
		DestroyWindow(window);
	}
	catch(const exception& e) {
		cerr << BRed <<  "Critical Error : " << e.what() << ResetColor <<'\n';
		exit(EXIT_FAILURE);
	}
}
