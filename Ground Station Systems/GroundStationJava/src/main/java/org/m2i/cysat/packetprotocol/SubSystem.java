package org.m2i.cysat.packetprotocol;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

public class SubSystem {

    String name;
    byte subSystemByte;
    HashMap<Byte, Command> commands;

    public SubSystem(String name, byte subSystemByte){
        this.name = name;
        this.subSystemByte = subSystemByte;
        commands = new HashMap<>();
    }

    public Command getCommandByByte(byte commandByte){
        return commands.get(commandByte);
    }

    public byte getSubSystemByte(){
        return this.subSystemByte;
    }

    public List<Command> getCommands() {
        return commands.values().stream().collect(Collectors.toList());
    }

    public List<String> getCommandStrings() {
        return commands.values().stream().map(v -> v.getCommandName()).collect(Collectors.toList());
    }

    public String getName(){
        return this.name;
    }

    public List<Command> getSendableCommands(){
        return this.commands.values().stream().filter(c -> c.getCommandByte()%2 == 1).collect(Collectors.toList());
    }

    public void addCommand(Byte commandByte, Command c){
        commands.put(commandByte, c);
    }
}
