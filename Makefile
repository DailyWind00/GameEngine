MAKEFLAGS += --no-print-directory
NAME = GameEngine

all: release

release:
	@cmake -B build -DCMAKE_BUILD_TYPE=Release
	@make -C build -j $(MAKEFLAGS)
	@mv build/$(NAME) .

debug:
	@cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++
	@make -C build -j $(MAKEFLAGS)
	@mv build/$(NAME) .

clean:
	@rm -rf build
	@rm -f  *.log

fclean: clean
	@rm -f $(NAME)

# Need to create script to re-download dependencies
# wipe: fclean
# 	@rm -rf dependencies

re: fclean all

.PHONY: all release debug clean fclean re