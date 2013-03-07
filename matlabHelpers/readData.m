function [Subject] = readData(SubjID)

dir = 'Results';
str = sprintf('%s/Subject-%d.dat', dir, SubjID);
fid = fopen(str, 'r');

Str = fgetl(fid);

numR = 50;

Subject.SubjID = Str;

for i=1:numR
    
    line = fgetl(fid);
    Subject.NumR(i) = str2num(line);
    
    % lottery 1
    line = fgetl(fid);
    Outcome1 = regexp(line, ',', 'split');
    for j=1:3; Subject.Outcome1(i,j)=str2num(Outcome1{j}); end
    
    line = fgetl(fid);
    Prob1 = regexp(line, ',', 'split');
    for j=1:3; Subject.Prob1(i,j)=str2num(Prob1{j}); end
    
    % lottery 2
    line = fgetl(fid);
    Outcome2 = regexp(line, ',', 'split');
    for j=1:3; Subject.Outcome2(i,j)=str2num(Outcome2{j}); end
    
    line = fgetl(fid);
    Prob2 = regexp(line, ',', 'split');
    for j=1:3; Subject.Prob2(i,j)=str2num(Prob2{j}); end
    
    % response
    line = fgetl(fid);
    Subject.Y(i) = str2num(line);
    
    % model posterior
    line = fgetl(fid);
    Post = regexp(line, ',', 'split');
    for j=1:6; Subject.ModelPost(i,j)=str2num(Post{j}); end
    
    % Response Time
    line = fgetl(fid);
    Subject.RT(i) = str2num(line);
    
end

fclose(fid);

% save results to .mat file
save(sprintf('Results/Subject-%d.mat', SubjID), 'Subject');