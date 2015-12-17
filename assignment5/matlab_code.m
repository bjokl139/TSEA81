%% Gather data
%Must be run on a Linux machine. Takes about 15 minutes on a laptop with an
%i7. OVERWIRTES CONTENT IN data FOLDER!!!
passengers = [5 10 20 30 40 50 60];

for it = passengers
    display(sprintf('one_cv with %d passengers', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=10000 -C threads one_cv_auto',it));
    
    system('./threads/lift_pthreads_one_cv');
    system(sprintf('cp one_cv.txt data/one_cv_%d.txt',it));
    
    system('taskset -c 0 ./threads/lift_pthreads_one_cv');
    system(sprintf('cp one_cv.txt data/one_cv_%d_onecpu.txt',it));
end

for it = passengers
    display(sprintf('many_cv with %d passengers', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=10000 -C threads many_cv_auto',it));
    
    system('./threads/lift_pthreads_many_cv');
    system(sprintf('cp many_cv.txt data/many_cv_%d.txt',it));
    
    system('taskset -c 0 ./threads/lift_pthreads_many_cv');
    system(sprintf('cp many_cv.txt data/many_cv_%d_onecpu.txt',it));
end

for it = passengers
    display(sprintf('single with %d passengers', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=10000 -C messages single_auto',it));
    
    system('./messages/lift_messages_single');
    system(sprintf('cp single_travels.txt data/single_travels_%d.txt',it));

    system('taskset -c 0 ./messages/lift_messages_single');
    system(sprintf('cp single_travels.txt data/single_travels_%d_onecpu.txt',it));
end

for it = passengers
    display(sprintf('multiple with %d passengers, 10 travels per message', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=1000 NUMBER_MESSAGES=10 -C messages multi_auto',it));
    
    system('./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_10.txt',it));

    system('taskset -c 0 ./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_10_onecpu.txt',it));

end

for it = passengers
    display(sprintf('multiple with %d passengers, 50 travels per message', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=200 NUMBER_MESSAGES=50 -C messages multi_auto',it));
    
    system('./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_50.txt',it));
    
    system('taskset -c 0 ./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_50_onecpu.txt',it));
end

for it = passengers
    display(sprintf('multiple with %d passengers, 100 travels per message', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=100 NUMBER_MESSAGES=100 -C messages multi_auto',it));
    
    system('./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_100.txt',it));
    
    system('taskset -c 0 ./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_100_onecpu.txt',it));
end
%% Load data
close all; clear all; clc;

load one_cv_5.txt
load one_cv_10.txt
load one_cv_20.txt
load one_cv_30.txt
load one_cv_40.txt
load one_cv_50.txt
load one_cv_60.txt

load many_cv_5.txt
load many_cv_10.txt
load many_cv_20.txt
load many_cv_30.txt
load many_cv_40.txt
load many_cv_50.txt
load many_cv_60.txt

load single_travels_5.txt
load single_travels_10.txt
load single_travels_20.txt
load single_travels_30.txt
load single_travels_40.txt
load single_travels_50.txt
load single_travels_60.txt

load multi_travels_5_10.txt
load multi_travels_10_10.txt
load multi_travels_20_10.txt
load multi_travels_30_10.txt
load multi_travels_40_10.txt
load multi_travels_50_10.txt
load multi_travels_60_10.txt

load multi_travels_5_50.txt
load multi_travels_10_50.txt
load multi_travels_20_50.txt
load multi_travels_30_50.txt
load multi_travels_40_50.txt
load multi_travels_50_50.txt
load multi_travels_60_50.txt

load multi_travels_5_100.txt
load multi_travels_10_100.txt
load multi_travels_20_100.txt
load multi_travels_30_100.txt
load multi_travels_40_100.txt
load multi_travels_50_100.txt
load multi_travels_60_100.txt

load one_cv_5_onecpu.txt
load one_cv_10_onecpu.txt
load one_cv_20_onecpu.txt
load one_cv_30_onecpu.txt
load one_cv_40_onecpu.txt
load one_cv_50_onecpu.txt
load one_cv_60_onecpu.txt

load many_cv_5_onecpu.txt
load many_cv_10_onecpu.txt
load many_cv_20_onecpu.txt
load many_cv_30_onecpu.txt
load many_cv_40_onecpu.txt
load many_cv_50_onecpu.txt
load many_cv_60_onecpu.txt

load single_travels_5_onecpu.txt
load single_travels_10_onecpu.txt
load single_travels_20_onecpu.txt
load single_travels_30_onecpu.txt
load single_travels_40_onecpu.txt
load single_travels_50_onecpu.txt
load single_travels_60_onecpu.txt

load multi_travels_5_10_onecpu.txt
load multi_travels_10_10_onecpu.txt
load multi_travels_20_10_onecpu.txt
load multi_travels_30_10_onecpu.txt
load multi_travels_40_10_onecpu.txt
load multi_travels_50_10_onecpu.txt
load multi_travels_60_10_onecpu.txt

load multi_travels_5_50_onecpu.txt
load multi_travels_10_50_onecpu.txt
load multi_travels_20_50_onecpu.txt
load multi_travels_30_50_onecpu.txt
load multi_travels_40_50_onecpu.txt
load multi_travels_50_50_onecpu.txt
load multi_travels_60_50_onecpu.txt

passengers = [5 10 20 30 40 50 60];
%% Mean travel times
mean_one_cv = [mean(mean(one_cv_5)), mean(mean(one_cv_10)), mean(mean(one_cv_20)),...
    mean(mean(one_cv_30)), mean(mean(one_cv_40)),...
    mean(mean(one_cv_50)), mean(mean(one_cv_60))];

mean_many_cv = [mean(mean(many_cv_5)), mean(mean(many_cv_10)), mean(mean(many_cv_20)),...
    mean(mean(many_cv_30)), mean(mean(many_cv_40)),...
    mean(mean(many_cv_50)), mean(mean(many_cv_60))];

mean_single_travels = [mean(mean(single_travels_5)), mean(mean(single_travels_10)), mean(mean(single_travels_20)),...
    mean(mean(single_travels_30)), mean(mean(single_travels_40)),...
    mean(mean(single_travels_50)), mean(mean(single_travels_60))];

mean_multi_travels_10 = [mean(mean(multi_travels_5_10)), mean(mean(multi_travels_10_10)), mean(mean(multi_travels_20_10)),...
    mean(mean(multi_travels_30_10)), mean(mean(multi_travels_40_10)),...
    mean(mean(multi_travels_50_10)), mean(mean(multi_travels_60_10))]/10;

mean_multi_travels_50 = [mean(mean(multi_travels_5_50)), mean(mean(multi_travels_10_50)), mean(mean(multi_travels_20_50)),...
    mean(mean(multi_travels_30_50)), mean(mean(multi_travels_40_50)),...
    mean(mean(multi_travels_50_50)), mean(mean(multi_travels_60_50))]/50;

mean_multi_travels_100 = [mean(mean(multi_travels_5_100)), mean(mean(multi_travels_10_100)), mean(mean(multi_travels_20_100)),...
    mean(mean(multi_travels_30_100)), mean(mean(multi_travels_40_100)),...
    mean(mean(multi_travels_50_100)), mean(mean(multi_travels_60_100))]/100;

figure(1)
plot(passengers,[mean_one_cv; mean_many_cv])
title('Genomsnittlig restid med trådar (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('En CV', 'En CV per våning')

figure(2)
plot(passengers,[mean_single_travels; mean_multi_travels_10; mean_multi_travels_50; mean_multi_travels_100])
title('Genomsnittlig restid med processer (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('1 resa per meddelande','10 resa per meddelande','50 resa per meddelande','100 resa per meddelande');

%% Standard deviations

std_one_cv = [mean(std(one_cv_5')), mean(std(one_cv_10')), mean(std(one_cv_20')),...
    mean(std(one_cv_30')), mean(std(one_cv_40')),...
    mean(std(one_cv_50')), mean(std(one_cv_60'))];

std_many_cv = [mean(std(many_cv_5')), mean(std(many_cv_10')), mean(std(many_cv_20')),...
    mean(std(many_cv_30')), mean(std(many_cv_40')),...
    mean(std(many_cv_50')), mean(std(many_cv_60'))];

std_single_travels = [mean(std(single_travels_5')), mean(std(single_travels_10')), mean(std(single_travels_20')),...
    mean(std(single_travels_30')), mean(std(single_travels_40')),...
    mean(std(single_travels_50')), mean(std(single_travels_60'))];

std_multi_travels_10 = [mean(std(multi_travels_5_10'/10)), mean(std(multi_travels_10_10'/10)), mean(std(multi_travels_20_10'/10)),...
    mean(std(multi_travels_30_10'/10)), mean(std(multi_travels_40_10'/10)),...
    mean(std(multi_travels_50_10'/10)), mean(std(multi_travels_60_10'/10))];

std_multi_travels_50 = [mean(std(multi_travels_5_50'/50)), mean(std(multi_travels_10_50'/50)), mean(std(multi_travels_20_50'/50)),...
    mean(std(multi_travels_30_50'/50)), mean(std(multi_travels_40_50'/50)),...
    mean(std(multi_travels_50_50'/50)), mean(std(multi_travels_60_50'/50))];

std_multi_travels_100 = [mean(std(multi_travels_5_100'/100)), mean(std(multi_travels_10_100'/100)), mean(std(multi_travels_20_100'/100)),...
    mean(std(multi_travels_30_100'/100)), mean(std(multi_travels_40_100'/100)),...
    mean(std(multi_travels_50_100'/100)), mean(std(multi_travels_60_100'/100))];

figure(3)
plot(passengers, [std_one_cv; std_many_cv])
title('Restidens standardavvikelse med trådar (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Genomsnittlig standardavvikelse')
legend('En CV', 'En CV per våning')

figure(4)
plot(passengers,[std_single_travels; std_multi_travels_10; std_multi_travels_50; std_multi_travels_100])
title('Restidens standardavvikelse med processer (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Genomsnittlig standardavvikelse')
legend('1 resa per meddelande','10 resa per meddelande','50 resa per meddelande','100 resa per meddelande');

std_one_cv_onecpu = [mean(std(one_cv_5_onecpu')), mean(std(one_cv_10_onecpu')), mean(std(one_cv_20_onecpu')),...
    mean(std(one_cv_30_onecpu')), mean(std(one_cv_40_onecpu')),...
    mean(std(one_cv_50_onecpu')), mean(std(one_cv_60_onecpu'))];

std_many_cv_onecpu = [mean(std(many_cv_5_onecpu')), mean(std(many_cv_10_onecpu')), mean(std(many_cv_20_onecpu')),...
    mean(std(many_cv_30_onecpu')), mean(std(many_cv_40_onecpu')),...
    mean(std(many_cv_50_onecpu')), mean(std(many_cv_60_onecpu'))];

std_single_travels_onecpu = [mean(std(single_travels_5_onecpu')), mean(std(single_travels_10_onecpu')), mean(std(single_travels_20_onecpu')),...
    mean(std(single_travels_30_onecpu')), mean(std(single_travels_40_onecpu')),...
    mean(std(single_travels_50_onecpu')), mean(std(single_travels_60_onecpu'))];

figure(14)
clf
hold on
plot(passengers,std_one_cv)
plot(passengers,std_many_cv)
ax = gca;
ax.ColorOrderIndex = 1;
plot(passengers,std_one_cv_onecpu,'--')
plot(passengers,std_many_cv_onecpu,'--')
title('Restidens standardavvikelse, trådar')
xlabel('Person')
ylabel('Standardavvikelse')
legend('1 CV, 4 kärnor','1 CV per våning, 4 kärnor','1 CV, 1 kärna','1 CV per våning, 1 kärna','Location','nw')
axis([5 60 0 4e4])
hold off

figure(15)
clf
hold on
plot(passengers,std_single_travels)
ax = gca;
ax.ColorOrderIndex = 1;
plot(passengers,std_single_travels_onecpu,'--')
title('Restidens standardavvikelse, processer')
xlabel('Person')
ylabel('Standardavvikelse')
legend('1 meddelande per resa, 4 kärnor','1 meddelande per resa, 1 kärna')
%axis([5 60 0 1.2e6])
hold off
%% Max travel time, INTETSÃ„GANDE!!!
max_one_cv = [max(max(one_cv_5)), max(max(one_cv_10)), max(max(one_cv_20)), ... 
    max(max(one_cv_30)), max(max(one_cv_40)), ... 
    max(max(one_cv_50)), max(max(one_cv_60))];

max_many_cv = [max(max(many_cv_5)), max(max(many_cv_10)), max(max(many_cv_20)), ... 
    max(max(many_cv_30)), max(max(many_cv_40)), ... 
    max(max(many_cv_50)), max(max(many_cv_60))];

max_single_travels = [max(max(single_travels_5)), max(max(single_travels_10)), max(max(single_travels_20)), ... 
    max(max(single_travels_30)), max(max(single_travels_40)), ... 
    max(max(single_travels_50)), max(max(single_travels_60))];

max_multi_travels_10 = [max(max(multi_travels_5_10)), max(max(multi_travels_10_10)), max(max(multi_travels_20_10)), ... 
    max(max(multi_travels_30_10)), max(max(multi_travels_40_10)), ... 
    max(max(multi_travels_50_10)), max(max(multi_travels_60_10))]/10;

max_multi_travels_50 = [max(max(multi_travels_5_50)), max(max(multi_travels_10_50)), max(max(multi_travels_20_50)), ... 
    max(max(multi_travels_30_50)), max(max(multi_travels_40_50)), ... 
    max(max(multi_travels_50_50)), max(max(multi_travels_60_50))]/50;

max_multi_travels_100 = [max(max(multi_travels_5_100)), max(max(multi_travels_10_100)), max(max(multi_travels_20_100)), ... 
    max(max(multi_travels_30_100)), max(max(multi_travels_40_100)), ... 
    max(max(multi_travels_50_100)), max(max(multi_travels_60_100))]/100;


max_one_cv_onecpu = [max(max(one_cv_5_onecpu)), max(max(one_cv_10_onecpu)), max(max(one_cv_20_onecpu)), ... 
    max(max(one_cv_30_onecpu)), max(max(one_cv_40_onecpu)), ... 
    max(max(one_cv_50_onecpu)), max(max(one_cv_60_onecpu))];

max_many_cv_onecpu = [max(max(many_cv_5_onecpu)), max(max(many_cv_10_onecpu)), max(max(many_cv_20_onecpu)), ... 
    max(max(many_cv_30_onecpu)), max(max(many_cv_40_onecpu)), ... 
    max(max(many_cv_50_onecpu)), max(max(many_cv_60_onecpu))];

max_single_travels_onecpu = [max(max(single_travels_5_onecpu)), max(max(single_travels_10_onecpu)), max(max(single_travels_20_onecpu)), ... 
    max(max(single_travels_30_onecpu)), max(max(single_travels_40_onecpu)), ... 
    max(max(single_travels_50_onecpu)), max(max(single_travels_60_onecpu))];

figure(5)
plot(passengers,[max_one_cv; max_many_cv])
title('Maximal restid med trådar (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('1 CV', '1 CV per våning')

figure(6)
plot(passengers,[max_single_travels; max_multi_travels_10; max_multi_travels_50; max_multi_travels_100])
title('Maximal restid med processer (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('1 resa per meddelande','10 resa per meddelande','50 resa per meddelande','100 resa per meddelande');

figure(11)
plot(passengers,[max_one_cv;max_many_cv;max_single_travels])
title('Maximal restid (4 kärnor)')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('Trådar, 1 CV','Trådar, 1 CV per våning','Processer, 1 meddelande per resa')

figure(12)
plot(passengers,[max_one_cv_onecpu;max_many_cv_onecpu;max_single_travels_onecpu])
title('Maximal restid (1 kärna)')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('Trådar, en händelsevariabel','Trådar, flera händelsevariabel','Processer, ett meddelande per resa')


figure(14)
clf
title('Genomsnittlig restid')
hold on
plot(passengers,mean_one_cv)
plot(passengers,mean_many_cv)
plot(passengers,mean_single_travels)
ax = gca;
ax.ColorOrderIndex = 1;
plot(-1,-1,'-k')
plot(-1,-1,'--k')
plot(passengers,mean_one_cv_onecpu,'--')
plot(passengers,mean_many_cv_onecpu,'--')
plot(passengers,mean_single_travels_onecpu,'--')
xlabel('Person')
ylabel('Restid [\mus]')
legend('Trådar, en händelsevariabel','Trådar, flera händelsevariabel','Processer, ett meddelande per resa','4 kärnor','1 kärna')
axis([5 60 0 2.5e4])
hold off
%% Mean travel time for each person
figure(7)
title('Medelrestid för var person, trådar')
hold on
plot(mean(one_cv_5'));
plot(mean(one_cv_10'));
plot(mean(one_cv_20'));
plot(mean(one_cv_30'));
plot(mean(one_cv_40'));
plot(mean(one_cv_50'));
plot(mean(one_cv_60'));
ax = gca;
ax.ColorOrderIndex = 1;
plot(-1,-1,'-k')
plot(-1,-1,'--k')
plot(mean(many_cv_5'),'--');
plot(mean(many_cv_10'),'--');
plot(mean(many_cv_20'),'--');
plot(mean(many_cv_30'),'--');
plot(mean(many_cv_40'),'--');
plot(mean(many_cv_50'),'--');
plot(mean(many_cv_60'),'--');
ax.YScale = 'log';
xlabel('Person')
ylabel('Restid [\mus]')
axis([0 60 0 30000]);
legend('5 personer','10 personer','20 personer','30 personer','40 personer','50 personer','60 personer','1 CV','1 CV per våning', 'Location', 'se')

hold off

figure(8)
clf;
title('Medelrestid för var person, processer (4 kärnor)')
hold on
plot(mean(single_travels_60'))
plot(mean(multi_travels_60_10')/10)
plot(mean(multi_travels_60_50')/50)
plot(mean(multi_travels_60_100')/100)
legend('1 resa per meddlenade','10 resor per meddlenade','50 resor per meddlenade','100 resor per meddlenade')
ax = gca;
ax.YScale = 'log';
xlabel('Person')
ylabel('Restid [\mus]')
axis([0 60 10 1000]);
hold off

%% One core
mean_one_cv_onecpu = [mean(mean(one_cv_5_onecpu)), mean(mean(one_cv_10_onecpu)), mean(mean(one_cv_20_onecpu)),...
    mean(mean(one_cv_30_onecpu)), mean(mean(one_cv_40_onecpu)),...
    mean(mean(one_cv_50_onecpu)), mean(mean(one_cv_60_onecpu))];

mean_many_cv_onecpu = [mean(mean(many_cv_5_onecpu)), mean(mean(many_cv_10_onecpu)), mean(mean(many_cv_20_onecpu)),...
    mean(mean(many_cv_30_onecpu)), mean(mean(many_cv_40_onecpu)),...
    mean(mean(many_cv_50_onecpu)), mean(mean(many_cv_60_onecpu))];

mean_single_travels_onecpu = [mean(mean(single_travels_5_onecpu)), mean(mean(single_travels_10_onecpu)), mean(mean(single_travels_20_onecpu)),...
    mean(mean(single_travels_30_onecpu)), mean(mean(single_travels_40_onecpu)),...
    mean(mean(single_travels_50_onecpu)), mean(mean(single_travels_60_onecpu))];

mean_multi_travels_10_onecpu = [mean(mean(multi_travels_5_10_onecpu)), mean(mean(multi_travels_10_10_onecpu)), mean(mean(multi_travels_20_10_onecpu)),...
    mean(mean(multi_travels_30_10_onecpu)), mean(mean(multi_travels_40_10_onecpu)),...
    mean(mean(multi_travels_50_10_onecpu)), mean(mean(multi_travels_60_10_onecpu))]/10;

figure(9)
clf;
hold on;
plot(passengers,[mean_one_cv; mean_many_cv])
ax = gca;
ax.ColorOrderIndex = 1;
plot(passengers,[mean_one_cv_onecpu; mean_many_cv_onecpu], '--')
title('Genomsnittlig restid med trådar')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('En CV, 4 kärnor', 'En CV per våning, 4 kärnor', 'En CV, 1 kärna', 'En CV per våning, 1 kärna')
hold off

figure(10)
clf;
hold on;
plot(passengers,[mean_single_travels; mean_multi_travels_10;])
ax = gca;
ax.ColorOrderIndex = 1;
plot(passengers,[mean_single_travels_onecpu; mean_multi_travels_10_onecpu],'--')
title('Genomsnittlig restid med processer')
xlabel('Antal passagerare')
ylabel('Restid [\mus]')
legend('1 resa per meddelande, fyra kärnor','10 resor per meddelande, fyra kärnor','1 resa per meddelande, en kärna','10 resor per meddelande, en kärna');
