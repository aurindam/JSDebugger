function setBreakpointInMain() {
    var args = new Object();
    args.command = "setbreakpoint";
    args.arguments = new Object();
    args.arguments.type = "function";
    args.arguments.target = "main";
    return args;
}

function setBreakpoint(line) {
    var args = new Object();
    args.command = "setbreakpoint";
    args.arguments = new Object();
    args.arguments.type = "script";
    args.arguments.target = mainWindow.filename;
    args.arguments.line = line;
    return args;
}
