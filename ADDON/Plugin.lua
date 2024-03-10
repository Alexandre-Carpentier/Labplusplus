
function lua_entrypoint(wxInst)
	print("@LUA entrypoint called...\n")

	-- call the builder (mandatory)
	print("@LUA start building the plugin\n")

	-- builder()
	-- 	return a builder instance
	builder1 = builder()

	builder1:ProduceIdentity("Keithley 2280S",2, "Voltage","Volt")
	-- ProduceIdentity("name", PLUGIN_ACCESS, "meas unit", "unit")
	-- 	Set the device name, access mode needed to be handled by core software, also add the measurement unit here.
	-- 	arg1: Name of the equipment
	-- 	arg2: PLUGIN_ACCESS READ=0 , WRITE=1, ALL=2
	-- 	arg3: type of measurements ex: voltage
	-- 	arg4: unit to attribute ex: Volt

	builder1:ProduceProtocol(4)
	
	-- ProducePanel(wxWindow*)
	-- 	Produce the panel (mandatory) to display controls/imgs/... lua_entrypoint receive
	-- 	automaticaly the wxWindow pointer object as wxInst.
	-- 	arg1: wxWindow*
	builder1:ProducePanel(wxInst)
	
	-- ProduceImage(wxWindow*)
	-- 	Add an image in the plugin panel if needed (optionnal). lua_entrypoint receive
	-- 	automaticaly the wxWindow pointer object as wxInst.
	-- 	arg1: wxWindow*
	builder1:ProduceImage(wxInst)
	
	-- AddPanelCtrl(CONTROLTYPE, length, height, "Text to write")
	-- 	Add a control (widget) to the current panel. ProducePanel(wxWindow*) must be call 
	-- 	before you add a control.
	-- 	arg1: CONTROLTYPE BUTTON=0, TXT=1, TXTBOX=2, SPINBUT=3
	-- 	arg2: length
	-- 	arg3: height
	-- 	arg4: name of the control	
	builder1:AddPanelCtrl(1, 400, 30, "initialisation alimentation")
	builder1:AddPanelCtrl(2, 200, 100, "*RST\\nOUTP:1\\n")
	builder1:AddPanelCtrl(1, 400, 30, "commande de lecture de l'appareil")
	builder1:AddPanelCtrl(2, 200, 100, "SOUR1:SENS?\n")
	builder1:AddPanelCtrl(1, 400, 30, "commande de consigne")
	builder1:AddPanelCtrl(2, 200, 200, "SOUR1:SET 10\n")
	builder1:AddPanelCtrl(1, 400, 30, "sequence de nettoyage")
	builder1:AddPanelCtrl(2, 200, 200, "OUTP:0*CLS\n")

	-- must return the builder object -> builder1
	-- as the plugin use it to handle behaviours and properties
	return builder1:GetObject()
end


function lua_init()
-- retrieve the current device built previously
device = builder1:GetObject();
-- use scpi
device:scpi_open("192.168.0.1")
device:scpi_write("*RST")
device:scpi_close("192.168.0.1")
end

function lua_set(setpoint)
print(setpoint)
end

function lua_read()
readpoint = 12
return readpoint
end

function lua_close()
	print("@LUA close called")
end