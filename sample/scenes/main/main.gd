extends Node

func _ready() -> void:
	print(HIDUSB.get_device_overviews())
	#var device := HIDUSB.open(0xfba5, 0x0001)
	#device.write(PackedByteArray([0, 1, 2, 3, 4]))
	#print(device.read_timeout(64, 1000).get_string_from_ascii())
