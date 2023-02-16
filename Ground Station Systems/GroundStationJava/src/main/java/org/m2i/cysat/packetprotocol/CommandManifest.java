package org.m2i.cysat.packetprotocol;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class CommandManifest {
    public static HashMap<Byte, SubSystem> subsystemMap;

    public CommandManifest() throws ParserConfigurationException, IOException, SAXException {
        subsystemMap = new HashMap<>();
        DocumentBuilderFactory fact = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = fact.newDocumentBuilder();
        InputStream is = this.getClass().getClassLoader().getResourceAsStream("command-manifest.xml");
        Document document = builder.parse(is);
        NodeList subsystems = document.getElementsByTagName("subsystem"); //Get all subsystem tags

        for (int i = 0; i < subsystems.getLength(); i++) {
            Node s = subsystems.item(i);
            String name = ((Element)s).getAttribute("name");
            String ssbytestring = ((Element)s).getAttribute("subsystemByte");
            byte ssbyte = getByteFromHex(ssbytestring);

            SubSystem sub = new SubSystem(name, ssbyte); // New SubSystem Definition

            NodeList commands = ((Element) s).getElementsByTagName("command");
            for(int j=0; j < commands.getLength(); j++){
                Node com = commands.item(j);
                Element ce = ((Element)com); // Command element

                Element req = (Element)ce.getElementsByTagName("request").item(0); // Request element
                Element resp = (Element)ce.getElementsByTagName("response").item(0); // Response element

                String commandName = ce.getAttribute("name");
                byte reqCByte = getByteFromHex(req.getAttribute("cbyte"));
                byte reqDataLength = getByteFromHex(req.getAttribute("datalength"));
                byte respCByte = getByteFromHex(resp.getAttribute("cbyte"));
                byte respDataLength = getByteFromHex(resp.getAttribute("datalength"));

                Command requestCommand = new Command.CommandImpl(commandName + " Request", reqCByte, reqDataLength);
                Command responseCommand = new Command.CommandImpl(commandName + " Response", respCByte, respDataLength);
                sub.addCommand(reqCByte, requestCommand);
                sub.addCommand(respCByte, responseCommand);
            }
            subsystemMap.put(new Byte(ssbyte), sub);
        }
    }

    public static List<String> getSubSystemNames(){
        List<String> names = new ArrayList<>();
        for(SubSystem s : subsystemMap.values()){
            names.add(s.name);
        }
        return names;
    }

    private byte getByteFromHex(String hex){
        String h = hex.replace("0x", "");
        return (byte)Integer.parseInt(h, 16);
    }
}
