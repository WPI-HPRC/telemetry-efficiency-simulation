% Clear the workspace variables
clearvars;
clc;
% OUR CONTROL VARIABLES:
% Whether to plot one line or three
multi_line = 1;
% Whether the data should have any sort of waveforms in it or not
noisy = 0;
% Whether to write the data or not
write_data = 1;
% SETUP:
% Number of iterations
number_steps = 500;
% Point where the function generator for each line changes
line1_change = round(0.5*number_steps);
line2_change = round(0.66*number_steps);
global line3_change;
line3_change = round(0.75*number_steps);
% Number of lines
num_lines = 1;
% Plot three lines if we want multiple lines
if multi_line
   num_lines = 3;
end
% Our value array
values(number_steps,num_lines) = 0;
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
% Set initial values:
if multi_line
   values(1,2) = 20;
   values (1,3) = line3_start;
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
   % If multiple lines
   if multi_line
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
hold on
plot(values);
hold off
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
