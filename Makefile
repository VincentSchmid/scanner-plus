docker-run:
	docker build -t scanner .
	docker run -it --device /dev/bus/usb/001/004:/dev/bus/usb/001/004 your_image_name