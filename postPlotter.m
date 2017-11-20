cor = zeros(10,1);
num = zeros(10,1);
a = zeros(10,10);

for i = 1:length(datas)
    res(i,:) = datas{i, end};
    try
    imgs{i, 1} = 255*datas{i, 1};
    imgs{i, 2} = 255*datas{i, 2};
    imgs{i, 3} = 255*datas{i, 3};
    imgs{i, 4} = 255*datas{i, 4};
    catch err
    end
%     imgs{i,1} = datas{i,1};
%     imgs{i,2} = datas{i,2};
%     imgs{i,3} = datas{i,3};
%     imgs{i,4} = datas{i,4};
%     imgs{i,5} = datas{i,5};
end
for i = 1:length(datas)
   [p, res_digit] = max( res(i, :));  res_digit = res_digit - 1;
   num(digit(i)+1) = num(digit(i)+1) + 1;
   if digit(i) ==  res_digit
      cor(digit(i)+1) = cor(digit(i)+1) + 1;
   end
   a = a + (res(i,:))' * res(i,:);
end

figure; plot(fits(:,1), movmean(fits(:, 2), 30)); %set(gca, 'YScale', 'log')

figure; bar(0:9, cor./num * 100)

figure; bar3(0:9, a);

 fig1 = figure(); ax1 = axes(fig1);
% bar(ax1, 0:9, res(n, :));  title(ax1, num2str(digit(n) ));
% 
% fig2 = figure(); ax2 = axes(fig2); 
% image(ax2, (reshape(inputBatch(n,:), [28 28]))');



fig4 = figure(); 
ax4 =subplot(2, 2, 1); ax5 =subplot(2, 2, 2); ax6 =subplot(2, 2, 3); ax7 =subplot(2, 2, 4); 
title(ax4, 'conv1'); title(ax5, 'conv1_1');title(ax6, 'conv1_2'); title(ax7, 'conv2');


bar(ax1, 0:9, res(n, :));  title(ax1, num2str(digit(n) ));image(ax4, imgs{n, 1});image(ax5, imgs{n, 2});image(ax6, imgs{n, 3});image(ax7, imgs{n, 4}); n = n + 1;
n=1;

