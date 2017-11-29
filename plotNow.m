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
    
    try
        
        
        [m_x, n_x] = size(x_);
        [m_y, n_y] = size(y_);
        if m_x == m_y
            y = [y; y_(length(y)+1:end, :)];
            x = [x; x_(length(x)+1:end)];
            for i = 1:n_y
                 set(plot1(i), 'XData', x, 'YData', y(:,i))
            end
        elseif n_x == n_y
            y = [y, y_(:, length(y)+1:end)];
            x = [x, x_(length(x)+1:end)];
            for i = 1:m_y
                 set(plot1(i), 'XData', x, 'YData', y(i,:));
            end
        else
            disp('dimension problem in plotNow');
        end
       
        axis([x(1) x(end) min(min(y)) max(max(y))])
    catch err
           rethrow(err)
    end
    drawnow;
end