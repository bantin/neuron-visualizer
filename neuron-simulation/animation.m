data = struct('positions', positions, 'voltage', voltage);
json_data = jsonencode(data);
fileID = fopen('data.json', 'w');
fprintf(fileID, json_data);
fclose(fileID);

[status, result] = system('start python -m http.server', '-echo');

web('http://localhost:8000/animation.html','-browser');