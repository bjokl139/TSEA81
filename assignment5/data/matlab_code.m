%% Gather data
passengers = [5 10 20 30 40 50 60];

for it = passengers
    display(sprintf('one_cv with %d passengers', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=10000 -C threads one_cv_auto',it));
    system('./threads/lift_pthreads_one_cv');
    system(sprintf('cp one_cv.txt data/one_cv_%d.txt',it));
end

for it = passengers
    display(sprintf('many_cv with %d passengers', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=10000 -C threads many_cv_auto',it));
    system('./threads/lift_pthreads_many_cv');
    system(sprintf('cp many_cv.txt data/many_cv_%d.txt',it));
end

for it = passengers
    display(sprintf('single with %d passengers', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=10000 -C messages single_auto',it));
    system('./messages/lift_messages_single');
    system(sprintf('cp single_travels.txt data/single_travels_%d.txt',it));
end

for it = passengers
    display(sprintf('multiple with %d passengers, 10 travels per message', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=1000 NUMBER_MESSAGES=10 -C messages multi_auto',it));
    system('./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_10.txt',it));
end

for it = passengers
    display(sprintf('multiple with %d passengers, 50 travels per message', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=200 NUMBER_MESSAGES=50 -C messages multi_auto',it));
    system('./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_50.txt',it));
end

for it = passengers
    display(sprintf('multiple with %d passengers, 100 travels per message', it))
    system(sprintf('make MAX_N_PERSONS=%d ITERATIONS=100 NUMBER_MESSAGES=100 -C messages multi_auto',it));
    system('./messages/lift_messages_multi');
    system(sprintf('cp multi_travels.txt data/multi_travels_%d_100.txt',it));
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
title('Travel time using threads (4 cores)')
xlabel('No. passengers')
ylabel('Mean time [\mus]')
legend('One CV', 'One CV per floor')

figure(2)
plot(passengers,[mean_single_travels; mean_multi_travels_10; mean_multi_travels_50; mean_multi_travels_100])
title('Travel time using message passing (4 cores)')
xlabel('No. passengers')
ylabel('Mean time [\mus]')
legend('1 travel per message','10 travels per message','50 travels per message','100 travels per message');

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
title('Travel time standard deviation using threads (4 cores)')
xlabel('No. passengers')
ylabel('Standard deviation')
legend('One CV', 'One CV per floor')

figure(4)
plot(passengers,[std_single_travels; std_multi_travels_10; std_multi_travels_50; std_multi_travels_100])
title('Travel time standard deviation using message passing (4 cores)')
xlabel('No. passengers')
ylabel('Standard deviation')
legend('1 travel per message','10 travels per message','50 travels per message','100 travels per message');


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
    max(max(multi_travels_50_10)), max(max(multi_travels_60_10))];

max_multi_travels_50 = [max(max(multi_travels_5_50)), max(max(multi_travels_10_50)), max(max(multi_travels_20_50)), ... 
    max(max(multi_travels_30_50)), max(max(multi_travels_40_50)), ... 
    max(max(multi_travels_50_50)), max(max(multi_travels_60_50))];

max_multi_travels_100 = [max(max(multi_travels_5_100)), max(max(multi_travels_10_100)), max(max(multi_travels_20_100)), ... 
    max(max(multi_travels_30_100)), max(max(multi_travels_40_100)), ... 
    max(max(multi_travels_50_100)), max(max(multi_travels_60_100))];



figure(5)
plot(passengers,[max_one_cv; max_many_cv])
title('Max travel time using threads (4 cores)')
xlabel('No. passengers')
ylabel('Max travel time')
legend('One CV', 'One CV per floor')

figure(6)
plot(passengers,[max_single_travels; max_multi_travels_10; max_multi_travels_50; max_multi_travels_100])
title('Max travel time using message passing (4 cores)')
xlabel('No. passengers')
ylabel('Max travel time')
legend('1 travel per message','10 travels per message','50 travels per message','100 travels per message');