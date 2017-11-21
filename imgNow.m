function imgNow(imgs)
persistent axes;
%     persistent ax1;
%     persistent ax2;
%     persistent ax3;
%     persistent ax4;
    persistent fig;
    if isempty(ax1)
        numImgs = length(imgs);
        fig = figure();
        axes = cell(1, numImgs);
        for i = 1:numImgs
           axes{i} = subplot( ceil(sqrt(numImgs)), ceil(sqrt(numImgs)), i);
        end
        
%       ax1 =subplot(2, 2, 1); 
%       ax2 =subplot(2, 2, 2); 
%       ax3 =subplot(2, 2, 3); 
%       ax4 =subplot(2, 2, 4); 

    end
    
    for i = 1:numImgs
           image(axes{i}, imgs{i});
        end
        
%    image(ax1, img1);
%    image(ax2, img2);
%    image(ax3, img3);
%    image(ax4, img4);
    drawnow;
end