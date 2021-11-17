close all;
clear all;

%------------- Unoptimized code test -------------

% gameoflife_unoptimized.c
% sizes = [200 300 400 500]
% for each method [-O,-O2,-O3,-Ofast]
% steps needed [456 508 1878 870 1670]
size200_unop = [1.212, 0.421, 0.424, 0.407];
size300_unop = [9.317, 3.568, 3.423, 3.003];
size400_unop = [7.546, 2.987, 3.117, 2.345];
size500_unop = [21.446, 8.307, 8.816, 6.925];

times1 = [size200_unop; size300_unop; size400_unop; size500_unop];
X1 = categorical({'200','300','400','500'});
X1 = reordercats(X1,{'200','300','400','500'});
figure(1)
b1 = bar(X1,times1);
b1(1).FaceColor = [51/255 255/255 51/255];
b1(2).FaceColor = [255/255 102/255 178/255];
b1(3).FaceColor = [51/255 153/255 255/255];
b1(4).FaceColor = [255/255 153/255 51/255];
ylim([0 22])
title('\fontsize{12}Execution times for different compiler flags (Unoptimized)')
ylabel('\fontsize{12}Time [s]')
xlabel('\fontsize{12}Size of world N')
legend('\fontsize{10}No flags','\fontsize{10}-O2','\fontsize{10}-O3','\fontsize{10}-Ofast','Location','northwest')
grid on

%------------- Optimized code test -------------

% gameoflife_optimized.c
% sizes = [200 300 400 500]
% for each method [-O,-O2,-O3,-Ofast]
% steps needed [456 508 1878 870 1670]
size200_op = [0.496, 0.249, 0.226, 0.242];
size300_op = [4.209, 1.774, 1.363, 1.486];
size400_op = [3.597, 1.724, 1.116, 1.265];
size500_op = [10.878, 3.975, 3.256, 3.551];

times2 = [size200_op; size300_op; size400_op; size500_op];
X2 = categorical({'200','300','400','500'});
X2 = reordercats(X2,{'200','300','400','500'});
figure(2)
b2 = bar(X2,times2);
b2(1).FaceColor = [51/255 255/255 51/255];
b2(2).FaceColor = [255/255 102/255 178/255];
b2(3).FaceColor = [51/255 153/255 255/255];
b2(4).FaceColor = [255/255 153/255 51/255];
ylim([0 12])
title('\fontsize{12}Execution times for different compiler flags (Optimized)')
ylabel('\fontsize{12}Time [s]')
xlabel('\fontsize{12}Size of world N')
legend('\fontsize{10}No flags','\fontsize{10}-O2','\fontsize{10}-O3','\fontsize{10}-Ofast','Location','northwest')
grid on

%------------- Parallel code test -------------

% sizes = [200 300 400 500]
% for each method [-O,-O2,-O3,-Ofast]
% steps needed [456 508 1878 870 1670]
size200_p = [0.328, 0.190, 0.163, 0.169];
size300_p = [2.184, 0.843, 0.831, 0.838];
size400_p = [1.663, 0.664, 0.593, 0.615];
size500_p = [4.758, 1.838, 1.624, 1.662];

%for 2 threads
times3 = [size200_p; size300_p; size400_p; size500_p];
X3 = categorical({'200','300','400','500'});
X3 = reordercats(X3,{'200','300','400','500'});
figure(3)
b3 = bar(X3,times3);
b3(1).FaceColor = [51/255 255/255 51/255];
b3(2).FaceColor = [255/255 102/255 178/255];
b3(3).FaceColor = [51/255 153/255 255/255];
b3(4).FaceColor = [255/255 153/255 51/255];
ylim([0 5])
title('\fontsize{12}Execution times for different compiler flags (Parallel, 2 threads)')
ylabel('\fontsize{12}Time [s]')
xlabel('\fontsize{12}Size of world N')
legend('\fontsize{10}No flags','\fontsize{10}-O2','\fontsize{10}-O3','\fontsize{10}-Ofast','Location','northwest')
grid on

%------------- Thread code test -------------

% sizes = [1000] --> 2692 steps
% for each method [-O3]
unoptimized = 60.091;   
optimized = 25.927;
thread2 = 12.233;   % 189% CPU
thread4 = 8.953;    % 322% CPU
thread8 = 8.557;    % 586% CPU

times4 = [unoptimized; optimized; thread2; thread4; thread8];
X4 = categorical({'Serial unop.', 'Serial op.','2 threads','4 threads','8 threads'});
X4 = reordercats(X4,{'Serial unop.', 'Serial op.','2 threads','4 threads','8 threads'});
figure(4)
b4 = bar(X4,times4);
b4.FaceColor = [51/255 255/255 51/255];
ylim([0 62])
title('\fontsize{12}Execution times for different amounts of threads (size N=1000)')
ylabel('\fontsize{12}Time [s]')
grid on

