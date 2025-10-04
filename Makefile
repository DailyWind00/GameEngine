MAKEFLAGS += --no-print-directory
NAME = GameEngine

all: release

release: dependencies
	@cmake -B build -DCMAKE_BUILD_TYPE=Release
	@make -C build -j $(MAKEFLAGS)
	@mv build/$(NAME) .

debug: dependencies
	@cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++
	@make -C build -j $(MAKEFLAGS)
	@mv build/$(NAME) .

dependencies:
	@./install_dependencies.sh

clean:
	@rm -rf build
	@rm -f  *.log

fclean: clean
	@rm -f $(NAME)

wipe: fclean
	@rm -rf dependencies

re: fclean all

.PHONY: all release debug dependencies clean fclean wipe re