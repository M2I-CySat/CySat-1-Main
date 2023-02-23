%Radiometer Parsing script
%Matthew E. Nelson
%Updated 12/16/2012

%This script uses the read_float_binary.m file to read in file written by
%GNURadio.  It will also read in the square law data and parse it as well
%It should work on both Matlab and Octave (you will ned the IO package for octave)

%Clear the workspace
clear all;

%Ask for the filenames for GNURadio and Square Law
gnuradio_file = uigetfile('*.*','Select the GNURadio data file');
disp('Importing Radiometer data...')
square_law = uigetfile('*.*','Select the Square_law data file');
disp('Importing Square Law data...')
%Call the read_float_binary script5
gnuradio = read_float_binary(gnuradio_file);
%Plot the data
% Create figure
subplot(2,1,1);
gnuradio = gnuradio*32;
plot(gnuradio);
title('N200 Data');
hold all;

% Create axes
%axes1 = axes('Parent',figure1,'YScale','log','YMinorTick','on','XGrid','on',...
%    'Position',[0.13 0.0413625304136253 0.775 0.883637469586375]);
%box(axes1,'on');
%hold(axes1,'all');

% Create semilogy
%semilogy(Y);

% Create xlabel
xlabel('Time');

% Create ylabel
ylabel('Uncalibrated Power');

% Create title
title('Power data from N200');
%Now parse the Square law file which is comma delimited.
%First, it helps to open the file
fid = fopen(square_law);
C = textscan(fid,'%s %s %s %s %s %s','delimiter',',');
[Date,VRAW,VdB,HRAW,HdB,HASH]=deal(C{:});

%We need to convert the cell to an array
v_pol = str2double(VRAW);
v_scale = v_pol(1:4.4:length(v_pol));
subplot(2,1,2);
plot(v_scale,'r')
title('Square Law Data');