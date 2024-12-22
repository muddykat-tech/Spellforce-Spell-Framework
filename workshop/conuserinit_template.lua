function OnMainScreenDataUpdate()
    local pVideoContainer
    pVideoContainer = Screen:ControlByName("<Cont>VideoSequenceController")
    if pVideoContainer ~= nil then
        return
    end
    
    local our_menu
    our_menu = tolua.cast(Screen:ControlByName("<Cont>cntMyTestContainer"), "CMnuContainer")
    if our_menu ~= nil then
        return
    end
    
    local form_data = 
    {
        0, 0, 0, 0;
        Name = "<Cont>cntMyTestContainer",
        AddTo = "Screen",
        HandleIfTransparent = true,
        Controls = 
        {
            {  40,  70, 0, 0; Type = "Label", Name = "<ctrl>lbMyTestLabel",    Font = kMnuFont_Type10, Mesh = "ui_oth_tooltip.msh", AutoSize = true}
        }
    }
    local form = UiCreateForm(form_data)
    local label1 = tolua.cast(form:ControlByName("<ctrl>lbMyTestLabel"), "CMnuLabel")
    label1:SetCaption("The Spellforce Spell Framework has been successfully installed. Restart the game for the changes to apply.")
end

function ConStartMenuUserInit()
    -- We have the data for sfsf.asi in a string (converted to escape characters with our luaconverter.py made by shovel_knight)
    local binary_data = "[WORKSHOP]"
    local ff = openfile("sfsf.asi", "r")
    if ff == nil then
        ff = openfile("sfsf.asi", "wb")
        write(ff, binary_data)
        closefile(ff)
        
        local screen_mnucont = tolua.cast(Screen, "CMnuContainer")
    
        screen_mnucont.OnDoDataUpdate = Application:MakeCallbackNotify ("OnMainScreenDataUpdate")
    else
        closefile(ff)
    end
end