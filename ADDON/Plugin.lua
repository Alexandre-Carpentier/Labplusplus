
function lua_entrypoint(wxInst)
	print("@LUA entrypoint called...\n")

	-- call the builder (mandatory)
	print("@LUA start building the plugin\n")

	-- builder()
	-- 	return a builder instance
	builder1 = builder()

	-- ProduceProtocol(SCPIMODE)
	-- 	Determine the protocol to use to link instrument. This this the
	--  different implementation available here. COM is full compatible
	--  RS232 driver implemented in the low level. TCP provide standard socket send/recv
	--  communication. USB is using VISA from the IVI foundation and support National Instrument.
	--  DAQMX is using National instrument Driver to communicate as well as VISA. 
	-- 	arg1: SCPIMODE COM=0 , TCP=1, USB=2, DAQMX=3, VISA=4 
	-- 	arg2: length
	-- 	arg3: height
	-- 	arg4: name of the control
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
	builder1:AddPanelCtrl(1, 400, 30, "initialisation de l'alimentation")
	builder1:AddPanelCtrl(2, 200, 200, "*RST\\nOUTP:1\\n")
	builder1:AddPanelCtrl(1, 400, 30, "commande de lecture de l'appareil")
	builder1:AddPanelCtrl(2, 200, 200, "SOUR1:SENS?\n")
	builder1:AddPanelCtrl(1, 400, 30, "commande de consigne")
	builder1:AddPanelCtrl(2, 200, 200, "SOUR1:SET 10\n")
	builder1:AddPanelCtrl(1, 400, 30, "sequence de nettoyage")
	builder1:AddPanelCtrl(2, 200, 200, "OUTP:0*CLS\n")

	-- must return the builder object -> builder1
	-- as the plugin use it to handle action and properties
	return builder1:GetObject()
end


function lua_init()

end

function lua_close()
	print("@LUA close called")
end