extends Node

func _ready() -> void:
	var result := HIDUSB.get_device_overviews()
	print(result)
