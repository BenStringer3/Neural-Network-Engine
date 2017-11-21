function plotNow(x_, y_)
    persistent x;
    persistent y;
    persistent plot1;
    persistent fig1;
    persistent ax1;
    if isempty(x)
       x = [];
       y = [];
       fig1 = figure();
       ax1 = axes(fig1);
       plot1 = plot(ax1, x_, y_);
    end
    
    
    [m, n] = size(y);
    if m > n
        y = [y; y_(length(y)+1:end, :)];
        x = [x; x_(length(x)+1:end)];
        for i = 1:n
             set(plot1(i), 'XData', x, 'YData', y(:,i));
        end
       
    else
        y = [y, y_(:, length(y)+1:end)];
        x = [x, x_(length(x)+1:end)];
        for i = 1:m
             set(plot1(i), 'XData', x, 'YData', y(i,:));
        end
       
    end
    
    
    
    axis([x(1) x(end) min(min(y)) max(max(y))]);
    drawnow;
end