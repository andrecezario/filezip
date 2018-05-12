%list
arquivo1 = fopen('comparisons_list.txt');
arquivo2 = fopen('comparisons_bst.txt');

V1 = fscanf(arquivo1,'%d %d',[2 Inf]);
V2 = fscanf(arquivo2,'%d %d',[2 Inf]);
fclose(arquivo1);
fclose(arquivo2);

c1 = size(V1,2);
%V1 = sort(V1,2);
%V2 = sort(V2,2);

for i=1:1:c1
    x1(i) = V1(1,i);
end

for i=1:1:c1
    y1(i) = V1(2,i);
end
%abb
c2 = size(V2,2);

for i=1:1:c2
    x2(i) = V2(1,i);
end

for i=1:1:c2
    y2(i) = V2(2,i);
end

%graph
figure;
plot(x1,y1,'r-',x2,y2,'b .')
grid on;
xlim([0 inf]);
ylim([0 inf]);
xlabel('Tamanho');
ylabel('Comparações');
legend('List','ABB');
title('List vs ABB');