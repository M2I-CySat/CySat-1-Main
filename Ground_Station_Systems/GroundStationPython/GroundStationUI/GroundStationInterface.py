import webbrowser
import serial
import time
import threading
from threading import Thread
import logging
import ctypes
from tkinter import *
import datetime
import binascii
import queue
import signal
import os
import struct  # for converting byte array to float
from tkinter.scrolledtext import ScrolledText
from tkinter import ttk, VERTICAL, HORIZONTAL, N, S, E, W, TOP, BOTH, BOTTOM
import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk
import importlib

packet_protocol_module = importlib.import_module('CySatPacket')
clock_module = importlib.import_module('CySatClock')
logging_module = importlib.import_module('CySatLogger')
global_module = importlib.import_module('CySatGlobal')
uart_module = importlib.import_module('UARTConnection')

logger = logging_module.CySatLogger(__name__)
cmd_dictionary = global_module.cmd_dictionary
sys_dictionary = global_module.sys_dictionary
sys_list = global_module.sys_list

DEBUG = global_module.DEBUG

class ConsoleUi:
    """Poll messages from a logging queue and display them in a scrolled text widget"""

    def __init__(self, frame):
        self.frame = frame
        # Create a ScrolledText widget
        self.scrolled_text = ScrolledText(frame, state='disabled', height=12)
        self.scrolled_text.grid(row=0, column=0, sticky=(N, S, W, E))
        self.scrolled_text.configure(font='TkFixedFont')
        self.scrolled_text.tag_config('INFO', foreground='black')
        self.scrolled_text.tag_config('DEBUG', foreground='gray')
        self.scrolled_text.tag_config('WARNING', foreground='orange')
        self.scrolled_text.tag_config('ERROR', foreground='red')
        self.scrolled_text.tag_config('CRITICAL', foreground='red', underline=1)
        # Create a logging handler using a queue
        self.log_queue = queue.Queue()
        self.queue_handler = logging_module.QueueHandler(self.log_queue)
        formatter = logging.Formatter('%(asctime)s: %(message)s')
        self.queue_handler.setFormatter(formatter)
        logger.addHandler(self.queue_handler)
        # Start polling messages from the queue
        self.frame.after(100, self.poll_log_queue)

    def display(self, record):
        msg = self.queue_handler.format(record)
        self.scrolled_text.configure(state='normal')
        self.scrolled_text.insert(tk.END, msg + '\n', record.levelname)
        self.scrolled_text.configure(state='disabled')
        # Autoscroll to the bottom
        self.scrolled_text.yview(tk.END)

    def poll_log_queue(self):
        # Check every 100ms if there is a new message in the queue to display
        while True:
            try:
                record = self.log_queue.get(block=False)
            except queue.Empty:
                break
            else:
                self.display(record)
        # Active polling?
        self.frame.after(100, self.poll_log_queue)
                    
# UI for Tab System
class TabUi:
    """Tab System for user to use satellite functionality based off subsystem tab selection"""
    def __init__(self, frame):
        self.frame = frame
        # Initialize tab (notebook) elements
        tab_parent = ttk.Notebook(frame)
        gen_tab = ttk.Frame(tab_parent)
        obc_tab = ttk.Frame(tab_parent)
        adcs_tab = ttk.Frame(tab_parent)
        sdr_tab = ttk.Frame(tab_parent)
        eps_tab = ttk.Frame(tab_parent)
        attitude_tab = ttk.Frame(tab_parent)
        eol_tab = ttk.Frame(tab_parent)

        # General Tab
        tab_parent.add(gen_tab, padding=3, text="General")
        self.button = ttk.Button(gen_tab, text="General Command 1", width=25, command=self.general_button1).place(x=20, y=20)
        self.button = ttk.Button(gen_tab, text="General Command 2", width=25, command=self.general_button2).place(x=20, y=70)
        self.button = ttk.Button(gen_tab, text="General Command 3", width=25, command=self.general_button3).place(x=20, y=120)
        self.button = ttk.Button(gen_tab, text="General Command 4", width=25, command=self.general_button4).place(x=20, y=170)
        self.button = ttk.Button(gen_tab, text="General Command 5", width=25, command=self.general_button5).place(x=210, y=20)
        self.button = ttk.Button(gen_tab, text="General Command 6", width=25, command=self.general_button6).place(x=210, y=70)
        self.button = ttk.Button(gen_tab, text="General Command 7", width=25, command=self.general_button7).place(x=210, y=120)
        self.button = ttk.Button(gen_tab, text="General Command 8", width=25, command=self.general_button8).place(x=210, y=170)

        # OBC Tab
        tab_parent.add(obc_tab, padding=3, text="OBC")
        self.button = ttk.Button(obc_tab, text="OBC Command 1", width=25, command=self.obc_button1).place(x=20, y=20)
        self.button = ttk.Button(obc_tab, text="OBC Command 2", width=25, command=self.obc_button2).place(x=20, y=70)
        self.button = ttk.Button(obc_tab, text="OBC Command 3", width=25, command=self.obc_button3).place(x=20, y=120)
        self.button = ttk.Button(obc_tab, text="OBC Command 4", width=25, command=self.obc_button4).place(x=20, y=170)
        self.button = ttk.Button(obc_tab, text="OBC Command 5", width=25, command=self.obc_button5).place(x=210, y=20)
        self.button = ttk.Button(obc_tab, text="OBC Command 6", width=25, command=self.obc_button6).place(x=210, y=70)
        self.button = ttk.Button(obc_tab, text="OBC Command 7", width=25, command=self.obc_button7).place(x=210, y=120)
        self.button = ttk.Button(obc_tab, text="OBC Command 8", width=25, command=self.obc_button8).place(x=210, y=170)

        # ADCS Tab
        tab_parent.add(adcs_tab, padding=3, text="ADCS")
        self.button = ttk.Button(adcs_tab, text="ADCS Command 1", width=25, command=self.adcs_button1).place(x=20, y=20)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 2", width=25, command=self.adcs_button2).place(x=20, y=70)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 3", width=25, command=self.adcs_button3).place(x=20, y=120)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 4", width=25, command=self.adcs_button4).place(x=20, y=170)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 5", width=25, command=self.adcs_button5).place(x=210, y=20)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 6", width=25, command=self.adcs_button6).place(x=210, y=70)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 7", width=25, command=self.adcs_button7).place(x=210, y=120)
        self.button = ttk.Button(adcs_tab, text="ADCS Command 8", width=25, command=self.adcs_button8).place(x=210, y=170)

        # SDR Tab
        tab_parent.add(sdr_tab, padding=3, text="SDR")
        self.button = ttk.Button(sdr_tab, text="SDR Command 1", width=25, command=self.sdr_button1).place(x=20, y=20)
        self.button = ttk.Button(sdr_tab, text="SDR Command 2", width=25, command=self.sdr_button2).place(x=20, y=70)
        self.button = ttk.Button(sdr_tab, text="SDR Command 3", width=25, command=self.sdr_button3).place(x=20, y=120)
        self.button = ttk.Button(sdr_tab, text="SDR Command 4", width=25, command=self.sdr_button4).place(x=20, y=170)
        self.button = ttk.Button(sdr_tab, text="SDR Command 5", width=25, command=self.sdr_button5).place(x=210, y=20)
        self.button = ttk.Button(sdr_tab, text="SDR Command 6", width=25, command=self.sdr_button6).place(x=210, y=70)
        self.button = ttk.Button(sdr_tab, text="SDR Command 7", width=25, command=self.sdr_button7).place(x=210, y=120)
        self.button = ttk.Button(sdr_tab, text="SDR Command 8", width=25, command=self.sdr_button8).place(x=210, y=170)

        # EPS Tab
        tab_parent.add(eps_tab, padding=3, text="EPS")
        self.button = ttk.Button(eps_tab, text="EPS Command 1", width=25, command=self.eps_button1).place(x=20, y=20)
        self.button = ttk.Button(eps_tab, text="EPS Command 2", width=25, command=self.eps_button2).place(x=20, y=70)
        self.button = ttk.Button(eps_tab, text="EPS Command 3", width=25, command=self.eps_button3).place(x=20, y=120)
        self.button = ttk.Button(eps_tab, text="EPS Command 4", width=25, command=self.eps_button4).place(x=20, y=170)
        self.button = ttk.Button(eps_tab, text="EPS Command 5", width=25, command=self.eps_button5).place(x=210, y=20)
        self.button = ttk.Button(eps_tab, text="EPS Command 6", width=25, command=self.eps_button6).place(x=210, y=70)
        self.button = ttk.Button(eps_tab, text="EPS Command 7", width=25, command=self.eps_button7).place(x=210, y=120)
        self.button = ttk.Button(eps_tab, text="EPS Command 8", width=25, command=self.eps_button8).place(x=210, y=170)

        tab_parent.add(attitude_tab, padding=3, text="Attitude")
        tab_parent.add(eol_tab, padding=3, text="EOL")
        tab_parent.pack(expand=1, fill='both')

    # Placeholder functions for future functionality additions
    # General Button Placeholder functions
    def general_button1(self):
        print("Hello")

    def general_button2(self):
        print("Hello")

    def general_button3(self):
        print("Hello")

    def general_button4(self):
        print("Hello")

    def general_button5(self):
        print("Hello")

    def general_button6(self):
        print("Hello")

    def general_button7(self):
        print("Hello")

    def general_button8(self):
        print("Hello")

    # OBC Button Placeholder functions
    def obc_button1(self):
        print("Hello")

    def obc_button2(self):
        print("Hello")

    def obc_button3(self):
        print("Hello")

    def obc_button4(self):
        print("Hello")

    def obc_button5(self):
        print("Hello")

    def obc_button6(self):
        print("Hello")

    def obc_button7(self):
        print("Hello")

    def obc_button8(self):
        print("Hello")

    # ADCS Button Placeholder functions
    def adcs_button1(self):
        print("Hello")

    def adcs_button2(self):
        print("Hello")

    def adcs_button3(self):
        print("Hello")

    def adcs_button4(self):
        print("Hello")

    def adcs_button5(self):
        print("Hello")

    def adcs_button6(self):
        print("Hello")

    def adcs_button7(self):
        print("Hello")

    def adcs_button8(self):
        print("Hello")

    # EPS Button Placeholder functions
    def sdr_button1(self):
        print("Hello")

    def sdr_button2(self):
        print("Hello")

    def sdr_button3(self):
        print("Hello")

    def sdr_button4(self):
        print("Hello")

    def sdr_button5(self):
        print("Hello")

    def sdr_button6(self):
        print("Hello")

    def sdr_button7(self):
        print("Hello")

    def sdr_button8(self):
        print("Hello")

    # SDR Button Placeholder functions
    def eps_button1(self):
        print("Hello")

    def eps_button2(self):
        print("Hello")

    def eps_button3(self):
        print("Hello")

    def eps_button4(self):
        print("Hello")

    def eps_button5(self):
        print("Hello")

    def eps_button6(self):
        print("Hello")

    def eps_button7(self):
        print("Hello")

    def eps_button8(self):
        print("Hello")

class FormUi:
    def __init__(self, frame):
        self.frame = frame
        self.uart = None
        # Create a combobox to select the logging level
        values = ['DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL']
        self.selected_cmd_list = []
        self.sys_id = 0
        self.cmd_id = 0

        row_helper = int(0)

        # Create com Port UI
        ttk.Label(self.frame, text='Port Settings').grid(pady=(10, 0), column=0, row=row_helper, columnspan=2)
        row_helper += 1
        port_label = ttk.Label(self.frame, text='COM Number').grid(column=0, row=row_helper, sticky=W)
        self.port_entry = ttk.Entry(self.frame)
        self.port_entry.grid(column=1, row=row_helper, sticky=tk.W+tk.E)
        row_helper += 1
        baud_label = ttk.Label(self.frame, text='Baud Rate').grid(column=0, row=row_helper, sticky=W)
        self.baud_entry = ttk.Entry(self.frame)
        self.baud_entry.grid(column=1, row=row_helper, sticky=tk.W+tk.E)
        row_helper += 1
        self.button = ttk.Button(self.frame, text="Connect", command=self.uart_init).grid(column=0, row=row_helper, sticky=tk.W+tk.E, columnspan=2)
        row_helper += 1
        self.button = ttk.Button(self.frame, text="Disconnect", command=self.uart_disconnect).grid(column=0, row=row_helper, sticky=tk.W+tk.E, columnspan=2)
        row_helper += 1

        ttk.Label(self.frame, text='Custom Packet Entry').grid(pady=(30, 0), column=0, row=row_helper, columnspan=2)
        row_helper += 1
        # Create dropdown field for type_id
        ttk.Label(self.frame, text='Sub-Type').grid(column=0, row=row_helper, sticky=W)
        self.sub_type = tk.StringVar()
        self.combobox_type = ttk.Combobox(self.frame, textvariable=sys_list, state='readonly', values=sys_list)
        self.combobox_type.grid(column=1, row=row_helper, sticky=tk.W+tk.E)
        self.combobox_type.set('Choose...')
        self.combobox_type.bind('<<ComboboxSelected>>', lambda event: self.update_cmd_list())
        row_helper += 1
        
        # Create dropdown field for cmd_id
        self.command_id = tk.StringVar()
        ttk.Label(self.frame, text='Command').grid(column=0, row=row_helper, sticky=W)
        self.combobox_cmd = ttk.Combobox(self.frame)
        self.combobox_cmd.grid(column=1, row=row_helper, sticky=tk.W+tk.E)
        self.combobox_cmd.configure(state='disabled')
        self.combobox_cmd.set('Choose...')
        self.combobox_cmd.bind('<<ComboboxSelected>>', lambda event: self.update_payload_options())
        row_helper += 1

        # Create dropdown field for payload input type selection
        self.payload_type = tk.StringVar()
        ttk.Label(self.frame, text='Payload Input Type').grid(column=0, row=row_helper, sticky=W)
        self.combobox_input_type = ttk.Combobox(self.frame, state='readonly', values=['ASCII', 'Hex Byte', 'Dec Byte'])
        self.combobox_input_type.grid(column=1, row=row_helper, sticky=tk.W+tk.E)
        self.combobox_input_type.configure(state='disabled')
        self.combobox_input_type.set('Choose...')
        self.combobox_input_type.bind('<<ComboboxSelected>>', lambda event: self.update_packet_button())
        row_helper += 1

        # Create text field for payload
        self.payload_text = tk.StringVar()
        ttk.Label(self.frame, text='Payload').grid(column=0, row=row_helper, sticky=W)
        self.payload_entry = ttk.Entry(self.frame, textvariable=self.payload_text, width=20)
        self.payload_entry.grid(column=1, row=row_helper, sticky=tk.W+tk.E)
        self.payload_entry.configure(state='disabled')
        row_helper += 1
        
        # Create a send packet button
        self.send_packet_button = ttk.Button(self.frame, text='Send Packet', width=15, command=self.send_custom_packet)
        self.send_packet_button.grid(column=0, row=row_helper, sticky=tk.W+tk.E, columnspan=2)
        self.send_packet_button.configure(state='disabled')
        row_helper += 1

        # Create buttons for programmed functions
        ttk.Label(self.frame, text='Default Packets').grid(pady=(30, 0), column=0, row=row_helper, columnspan=2)
        row_helper += 1
        self.button = ttk.Button(self.frame, text='Ping', width=25, command=self.satellite_ping)
        self.button.grid(column=0, row=row_helper, sticky=tk.W+tk.E, columnspan=2)
        row_helper += 1
        self.button = ttk.Button(self.frame, text='Shutoff Beacon', width=25, command=self.shutoff_beacon)
        self.button.grid(column=0, row=row_helper, sticky=tk.W+tk.E, columnspan=2)
        row_helper += 1
        self.button = ttk.Button(self.frame, text='EPS Battery Voltage', width=25, command=self.eps_battery)
        self.button.grid(column=0, row=row_helper, sticky=tk.W+tk.E, columnspan=2)
        row_helper += 1

        # Add CySat Info
        ttk.Label(self.frame, text='CySat Info').grid(pady=(30, 0), column=0, row=row_helper, columnspan=2)
        row_helper += 1
        self.button = ttk.Button(self.frame, text='About CySat', width=25, command=self.about_cysat)
        self.button.grid(column=0, row=row_helper, sticky=tk.W+tk.E+tk.S, columnspan=2)
        row_helper += 1
        self.button = ttk.Button(self.frame, text='Manual', width=25, command=self.cysat_manual)
        self.button.grid(column=0, row=row_helper, sticky=tk.W+tk.E+tk.S, columnspan=2)
        row_helper += 1

        # Image Insertion
        load = Image.open("cysat_logo.png")
        resize_load = load.resize((120, 120))
        render = ImageTk.PhotoImage(resize_load)
        img = Label(self.frame, image=render)
        img.image = render
        img.place(x=65, y=525)

    # Directs User to M:2:I CySat Webpage
    def about_cysat(self):
        new = 1
        url = "https://m2i.aere.iastate.edu/cysat/"
        webbrowser.open(url, new=new)

    # Directs User to Manual PDF Location
    def cysat_manual(self):
        new = 1
        url = "http://sdmay20-49.sd.ece.iastate.edu/docs/ground_station_manual.pdf"
        webbrowser.open(url, new=new)

    def update_cmd_list(self):

        # Reset state of UI elements 
        self.send_packet_button.configure(state='disabled')
        self.combobox_input_type.configure(state='disabled')
        self.combobox_input_type.set('Choose...')
        self.payload_entry.configure(state='disabled')
        self.payload_text.set('')

        # Get system id from selection
        type_drop_down_str = sys_list[self.combobox_type.current()]
        for key, value in sys_dictionary.items():
            if(value == type_drop_down_str):
                self.sys_id = key
                break

        # Add command descriptions to list for cmd dropdown
        cmd_list = cmd_dictionary[self.sys_id]
        dropdown_cmd_list = []
        for key, value in cmd_list.items():
            if(value.cmd_sendable == 1):
                dropdown_cmd_list.append(value.cmd_description)
        self.selected_cmd_list = dropdown_cmd_list

        # Enable the cmd dropdown
        self.combobox_cmd.configure(state='readonly', values=dropdown_cmd_list)
        self.combobox_cmd.set('Choose...')

    def update_payload_options(self):
        # Reset state of UI elements 
        self.send_packet_button.configure(state='disabled')
        self.combobox_input_type.configure(state='disabled')
        self.combobox_input_type.set('Choose...')
        self.payload_entry.configure(state='disabled')
        self.payload_text.set('')

        # Get list of command objects for that system type
        cmd_list = cmd_dictionary[self.sys_id]

        # Get string of command selected
        cmd_desc = self.selected_cmd_list[self.combobox_cmd.current()]

        # Look for ID of selected command
        for key, value in cmd_list.items():
            if(value.cmd_description == cmd_desc):
                self.cmd_id = key
                break

        # Check if cmd requires payload or not and enable UI
        cmd = cmd_list[self.cmd_id]
        if(cmd.cmd_has_payload == 1):
            self.combobox_input_type.configure(state='readonly')
            self.payload_entry.configure(state='!disabled')
        else:
            self.send_packet_button.configure(state='!disabled')

    def update_packet_button(self):
        # Update Packet Button Settings
        self.send_packet_button.configure(state='!disabled')
        if(self.combobox_input_type.current() == 0):
            self.payload_text.set('hello')
        elif(self.combobox_input_type.current() == 1):
            self.payload_text.set('68,65,6C,6C,6F')
        else:
            self.payload_text.set('104,101,108,108,111')

    def send_custom_packet(self):
        if(self.uart != None or DEBUG == 1):
            payload_bytes = bytearray()
            if(len(self.payload_text.get()) > 0):
                if(self.combobox_input_type.current() == 0):
                    payload_bytes = bytearray(self.payload_text.get(), 'utf-8')
                elif(self.combobox_input_type.current() == 1):
                    byte_list = self.payload_text.get().split(',')
                    for i in byte_list:
                        payload_bytes.append(int(i, 16))
                else:
                    byte_list = self.payload_text.get().split(',')
                    for i in byte_list:
                        payload_bytes.append(int(i, 10))

            packet = packet_protocol_module.create_packet(self.sys_id, self.cmd_id, payload_bytes)
            packet_bytes = packet_protocol_module.package_packet(packet)
            uart_module.gs_uart_write(self.uart, packet_bytes)
            logger.log_packet_sent(packet, payload_bytes, sys_dictionary, cmd_dictionary)
        else:
            messagebox.showinfo("Connection Status", "Connect to Serial Port First")
            
    # Control for Ping Button 
    def satellite_ping(self):
        if(self.uart != None or DEBUG == 1):
            payload_bytearray = bytearray()
            packet = packet_protocol_module.create_packet(global_module.OBC, 0x01, payload_bytearray)
            packet_bytes = packet_protocol_module.package_packet(packet)
            uart_module.gs_uart_write(self.uart, packet_bytes)
            logger.log_packet_sent(packet, payload_bytearray, sys_dictionary, cmd_dictionary)
        else:
            messagebox.showinfo("Connection Status", "Connect to Serial Port First")
        
    # Control for Beacon Shutoff Button 
    def shutoff_beacon(self):
        if(self.uart != None or DEBUG == 1):
            payload_bytearray = bytearray()
            packet = packet_protocol_module.create_packet(global_module.OBC, 0x03, payload_bytearray)
            packet_bytes = packet_protocol_module.package_packet(packet)
            uart_module.gs_uart_write(self.uart, packet_bytes)
            logger.log_packet_sent(packet, payload_bytearray, sys_dictionary, cmd_dictionary)
        else:
            messagebox.showinfo("Connection Status", "Connect to Serial Port First")
        
    # Control for Battery Voltage Button 
    def eps_battery(self):
        if(self.uart != None or DEBUG == 1):
            payload_bytearray = bytearray()
            packet = packet_protocol_module.create_packet(global_module.EPS, 0x01, payload_bytearray)
            packet_bytes = packet_protocol_module.package_packet(packet)
            uart_module.gs_uart_write(self.uart, packet_bytes)
            logger.log_packet_sent(packet, payload_bytearray, sys_dictionary, cmd_dictionary)
        else:
            messagebox.showinfo("Connection Status", "Connect to Serial Port First")

    # Control for Port disconnecting with error handling
    def uart_disconnect(self):
        try:
            self.reader_thread.raise_exception()
            print("UART Read Thread Stopped")
            self.uart.close()
            self.uart = None
            print("UART Port Closed")
            messagebox.showinfo("Connection Status", "Port Disconnected")
            ttk.destroy()
        except AttributeError:
            print
            "Closed without using Disconnect"

    # UART Port initialization
    def uart_init(self):
        
        self.port = "COM" + self.port_entry.get()
        self.baud = self.baud_entry.get()

        print(self.port, self.baud)

        try:
            uart = serial.Serial(self.port, self.baud, timeout=1)
        except Exception as e:
            messagebox.showinfo("Connection Status", "No Such Port Detected: " + str(self.port) + "\nPlease Input Port as so: #")
            print("No UART Port Connected")
            return
        
        if uart.is_open:
            messagebox.showinfo("Connection Status", "Successfully Connected to Port: " + str(self.port))
            print("UART Port Open")
            
            # pass UART port to form class
            self.uart = uart

            # ESPECIALLY NOT HERE HOLY MOLY

            # start reader thread
            reader_thread = uart_module.uart_reader(uart, logger)
            self.reader_thread = reader_thread
            print("UART Read Thread Started")
            reader_thread.start()
        else:
            print("UART Error! (Port probably not configured properly)")

class App:
    def __init__(self, root):
        self.root = root
        root.title('CySat Ground Station')
        root.columnconfigure(0, weight=1)
        root.rowconfigure(0, weight=1)
        # Create the panes
        vertical_pane = ttk.PanedWindow(self.root, orient=VERTICAL)
        vertical_pane.grid(row=0, column=0, sticky="nsew")
        horizontal_pane = ttk.PanedWindow(vertical_pane, orient=HORIZONTAL)
        vertical_pane.add(horizontal_pane)

        # Create the frames
        # Form Frame
        form_frame = ttk.Labelframe(horizontal_pane, text="Communication Control")
        form_frame.columnconfigure(1, weight=1)
        horizontal_pane.add(form_frame, weight=1)
        form_frame.pack(side=LEFT, fill="both")

        # Preset Tab Sizing
        style = ttk.Style()
        current_theme = style.theme_use()
        style.theme_settings(current_theme, {"TNotebook.Tab": {"configure": {"padding": [25, 5]}}})
        
        # Tab Frame
        tab_frame = ttk.Labelframe(horizontal_pane, height=155, text="Subsystem Selection")
        tab_frame.columnconfigure(0, weight=1)
        tab_frame.rowconfigure(0, weight=1)
        horizontal_pane.add(tab_frame, weight=50)
        tab_frame.pack(side=TOP, fill="both", expand="yes")

        # Console Frame
        console_frame = ttk.Labelframe(horizontal_pane, height=565, text="Log")
        console_frame.columnconfigure(0, weight=1)
        console_frame.rowconfigure(0, weight=1)
        horizontal_pane.add(console_frame, weight=50)
        console_frame.pack(side=BOTTOM, fill="both", expand="yes")

        # Initialize all frames
        self.console = ConsoleUi(console_frame)
        self.form = FormUi(form_frame)
        self.tab = TabUi(tab_frame)
        self.clock = clock_module.Clock(logger)
        self.clock.start()
        
        # Close Main GUI
        self.root.protocol('WM_DELETE_WINDOW', self.quit)
        self.root.bind('<Control-q>', self.quit)
        signal.signal(signal.SIGINT, self.quit)

    # Close GUI panel and established uart connection
    def quit(self, *args):
        self.clock.stop()
        try:
            self.form.reader_thread.raise_exception()
        except:
            None
        try:
            self.form.uart.close()
        except:
            None
        self.root.destroy()

def main():
    logging.basicConfig(level=logging.DEBUG)
    root = tk.Tk()
    root.geometry("1280x720")  # Width x Height
    global_module.populate_global_variables()
    app = App(root)
    app.root.mainloop()

if __name__ == "__main__":
    main()
