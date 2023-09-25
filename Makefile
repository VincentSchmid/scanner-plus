endpoint-test:
	pytest test/test.py

tests:
	cmake --build /home/vnc/dev/scanner-plus/build --config Debug --target scanner_tests
	./build/scanner_tests

stop-services:
	sudo systemctl stop scanner_server.service
	sudo systemctl stop scanner_telegram_server.service

start-services:
	sudo systemctl start scanner_server.service
	sudo systemctl start scanner_telegram_server.service

run:
	./build/scanner

deploy:
	sudo systemctl stop scanner_server.service
	cmake --build build --config Release --target scanner
	cp -p build/scanner /usr/local/bin/scanner
	sudo systemctl start scanner_server.service
