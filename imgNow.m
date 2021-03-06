function imgNow(datas, trueImg, results, iter)
    persistent imgAxes;
    persistent fig1;
    persistent fig2;
    persistent barAxes;
    branches = 2;
    depth = 3;
    numImgs = length(datas);
    if isempty(imgAxes) || ~ishandle(imgAxes{1})
            fig2 =figure();
            barAxes = cell(1, 2);
            barAxes{1} = subplot('position', [0.1, 0.1, 0.8, 0.6]);
            barAxes{2} = subplot('position', [0.4, 0.95-0.2, 0.2, 0.2]);
        ii = 1;
        fig1 = figure();
        imgAxes = cell(1, numImgs);
%         for i = 1:numImgs
%            axes{1} = subplot( ceil(sqrt(numImgs)), ceil(sqrt(numImgs)), i);
%        end
        w = 1/(ceil(sqrt(numImgs)));
        
        for i = 1:ceil(sqrt(numImgs))
            for j = 1:ceil(sqrt(numImgs))
                %[left bottom width height]
               	imgAxes{ii} = subplot('position', [(j - 1)*w,  1 - i*w, w-0.01, w-0.01]); ii = ii+1;            
            end
        end
        for i = 1:length(imgAxes)
             set(imgAxes{i}, 'YTickLabel', []);
             set(imgAxes{i}, 'XTickLabel', []);
        end
        drawnow;
    end
    
    %PLOT THE IMAGES
        for i = 1:numImgs
            if ishandle(imgAxes{i})
               %image(axes{i}, mat2gray(datas{i}, [0 1]));
               imshow(datas{i}, 'Parent', imgAxes{i});
                set(imgAxes{i}, 'YTickLabel', []);
             set(imgAxes{i}, 'XTickLabel', []);
            end
        end
      %PLOT THE BAR PLOT
      bar(barAxes{1}, 0:9, results * 100);
      set(barAxes{1}, 'ylim', [0 100]);
    imshow(trueImg, 'Parent', barAxes{2});
    title( barAxes{2}, ['original image', num2str(iter)]);
    title( barAxes{1}, 'Network Results');
    ylabel( barAxes{1}, '% confidence');
    xlabel( barAxes{1}, 'Digit');
    drawnow;
end