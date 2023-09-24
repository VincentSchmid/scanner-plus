endpoint-test:
	pytest test/test.py

tests:
	/opt/homebrew/bin/cmake --build /Users/vinc/dev/scanner-plus/build --config Debug --target scanner_tests
	cp ./build/scanner_tests ./scanner_tests
	./scanner_tests
