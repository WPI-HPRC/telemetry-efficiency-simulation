% Clear the workspace variables
clearvars;
clc;

% OUR CONTROL VARIABLES:
% Whether to plot one line or three
multi_line = 1;
% Whether to include different frequency lines or not
diff_freq = 1;
% Whether the data should have any sort of waveforms in it or not
noisy = 0;
% Whether to write the data or not
write_data = 1;
% Whether to read/graph received data or not
receive_data = 0;


% SETUP:
% Number of iterations
number_steps = 500;
% Point where the function generator for each line changes
line1_change = round(0.5*number_steps);
line2_change = round(0.66*number_steps);
global line3_change;
line3_change = round(0.75*number_steps);
line4_change = round(0.6*number_steps/2);
line5_change = round(0.4*number_steps/3);
line6_change = round(0.55*number_steps);

% Number of lines
num_lines = 1;

% Plot three lines if we want multiple lines
if multi_line
   num_lines = 3;
end

% Plot all six lines if we want variable frequency
if diff_freq
    num_lines = 6;
end

% Our value array
values(number_steps,num_lines) = 0;
received_Data(number_steps,num_lines*2) = 0;
if receive_data 
    received_Data = csvread('ReceivedData.csv');
end


% DATA GENERATION
% Our starting positions:
line2_start = 20;
global line3_start;
line3_start = 5;

% Our functions:
% Iterative proportional slope functions
f_line1_1 = 30/line1_change;
f_line2_1 = -50/line2_change;
f_line2_2 = 25/(number_steps - line2_change);
% f_line3_1 curve generator is complex and at the very bottom
f_line4 = 40/line4_change;
f_line5_1 = -30/line5_change;
f_line5_2 = 50/line5_change;
f_line6 = 45/line6_change;

% Set initial values:
if multi_line || diff_freq
   values(1,2) = 20;
   values (1,3) = line3_start;
end

if diff_freq
    values(1,4) = -30;
    values(1,5) = 10;
    values(1,6) = -35;
end

for i = 2:1:number_steps
   % For line 1
   % If before the line changer value
   if (i < line1_change)
       values(i,1) = values(i-1,1) + f_line1_1;
   % After the line changer value
   else
       values(i,1) = values(i-1,1);
   end

   % If multiple lines or different frequencies
   if ~multi_line || ~diff_freq
       % Skip next data
       continue;
   end

   % For line 2
   if (i < line2_change)
       values(i,2) = values(i-1,2) + f_line2_1;
   else
       values(i,2) = values(i-1,2) + f_line2_2;
   end
   % For line 3
   if (i < line3_change)
       values(i,3) = f_line3_1(i);
   else
       values(i,3) = values(i-1,3);
   end

   % If we have different frequencies for our lines
   if diff_freq
       % For line 4
       % Initializer for 2
       if i == 2
           values(i,4) = values(i-1,4) + f_line4/2;
       end
       % Remainder count and guard for number bound
       if (rem(i,2) == 0) && (i > 2)
           values(i,4) = values(i-2,4) + f_line4;
       % if for elseif to prevent initialization override
       elseif (i > 2)
           values(i,4) = missing;
       end
       % For line 5
       % Remainder count and guard for number bound
       if (rem(i,3) == 1) && (i > 3)
           if i < line5_change
               values(i,5) = values(i-3,5) + f_line5_1;
           else
               values(i,5) = values(i-3,5) + f_line5_2;
           end
       else
           values(i,5) = missing;
       end
       % For line 6
       values(i,6) = values(i-1,6) + f_line6;
   end

   % If noise on
   if noisy
       % For each line
       for j = 1:1:num_lines
           % Add our sine generation value
           values(i,j) = values(i,j) + noise_gen(i);
       end
   end
end


% DATA PLOTTING
figure(1)
hold on
plot(values);
if receive_data
    plot(received_Data);
end
hold off

if receive_data
    figure(2)
    plot(received_Data-values);
end


% DATA WRITING TO FILE
if write_data
   % Our file name:
   filename = 'Generated_Data.csv';
  
   recycle on % Send to recycle bin instead of permanently deleting.
   delete(filename); % Delete (send to recycle bin).
  
   % Now write new file with none of the old/existing stuff in there.
   csvwrite(filename, values);
end


% OUR FUNCTIONS:
% Function for generating the line 3 curve:
function datapoint = f_line3_1(x)
   global line3_change;
   global line3_start;
   b = line3_change;
   % y_0 is the y-position when x = 0
   y_0 = line3_start;
   c = 40;
   % a is defined that no matter b or c, the curve is at point (0, y_0)
   a = -(c-y_0)/(b^2);
   datapoint = a*(x - b)^2 + c;
end

% Function for generating the noise
function noise_unit = noise_gen(x)
   noise_unit = sin(x);
end
