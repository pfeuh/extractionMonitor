void #funcname#(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        #varname# +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        #varname# -=1;
    menu.printVariable(#varname#);
}

