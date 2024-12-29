MAKEFLAGS += --no-print-directory

all:
	@cmake -B build
	@make -C build -j $(MAKEFLAGS)
	@mv build/test .

clean:
	@rm -rf build

fclean: clean
	@rm -f test

re: fclean all

.PHONY: all clean fclean re