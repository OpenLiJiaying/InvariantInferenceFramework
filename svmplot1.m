clc;
clear;
close all;

path_root = '~/demo';
path_iter = [path_root, '/data/iternum'];
path_lines = [path_root, '/data/lines'];
path_points = [path_root, '/data/points'];

iterfid = fopen(path_iter);
ITER = fscanf(iterfid, '%d\n', 1);
%fprintf('ITER= %d', ITER);
fclose(iterfid);


linesfid = fopen(path_lines);

for j = 1:ITER
    clf;
    
    [pointsfid, fmsg] = fopen([path_points, num2str(j)]);

    
    points = zeros(1,3);
    points = fscanf(pointsfid, '%d 1:%d 2:%d\n', 3);
    
    while feof(pointsfid) ~= 1
        point = fscanf(pointsfid, '%d 1:%d 2:%d\n', 3);
        points = [points, point];
    end
    points = points';
    fclose(pointsfid);
    N = size(points, 1);
    
    label = points(:, 1);
    data = points(:, 2:3);
    
    for i = 1:length(label)
        if label(i) == 1
            plot(data(i, 1), data(i, 2), 'b+');
        else
            plot(data(i, 1), data(i, 2), 'ro');
        end
        hold on;
    end
    
    
%    fprintf('ITER= %d', ITER);
    xlabel('X');ylabel('Y');
    axis([-100, 100, -100, 100]);
    set(gca,'XTickMode','manual','XTick',-100:10:100);
    set(gca,'YTickMode','manual','YTick',-100:10:100);
    
    x = -100:0.2:100;
    y = x;
    plot(zeros(1, length(x)), y, 'c');
    plot(x, zeros(1, length(y)), 'r');
    pause(0.8);
    
    
    para = fscanf(linesfid, '%d %f %f %f\n', 4);
    if para(3) == 0
        A = para(4) / para(2);
        xb = ones(size(y)) * A;
        xc = ones(size(y)) * (A + 1);
        xd = ones(size(y)) * (A - 1);
        plot(xb, y, 'b.');
        plot(xc, y, 'b--');
        plot(xd, y, 'b--');
    else
        A = -1 * para(2) / para(3);
        B = -1 * para(4) / para(3);
        C =      (-1 * para(4) + 1) / para(3);
        D =      (-1 * para(4) - 1) / para(3);
        zb = A * x  + B;
        zc = A * x  + C;
        zd = A * x  + D;
        plot(x, zb, 'b.');
        plot(x, zc, 'b--');
        plot(x, zd, 'b--');
    end    
    pause(0.8);
end
fclose(linesfid);
