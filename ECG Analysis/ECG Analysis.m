clear all;
close all;

load('Time.mat');
load('ECG.mat');
time=Time.(1);
ecg=ECG.(1);
plot(time,ecg);
title("ECG Signal");
xlabel("Milliseconds");
ylabel("A0");