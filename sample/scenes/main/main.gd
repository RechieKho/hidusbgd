extends Node

func _ready() -> void:
	for overview in HIDUSB.get_device_overviews():
		printt(overview.product_string, overview.vendor_id, overview.product_id)
	#var device := HIDUSB.open(0xfba5, 0x1000)
	#var result := (device.get_input_report(0, 8))
	#var report_id := result[0]
	#var data := result.slice(1)
	#print(report_id)
	#print(data)
	#print(data.to_int64_array())
	
