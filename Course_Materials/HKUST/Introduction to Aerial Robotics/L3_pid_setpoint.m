close all;
clear all;
clc;

% PID
Kp = 10;
Ki = 0;
Kd = 0;

% System model
dt = 0.01;
A = [1 dt; 0 0.95];
B = [dt*dt/2 ; dt];

% Initial values
ie = 0;
pe = 0;
t = 0;
x = [0;0];
% Figure
h = figure;
xlabel('t (sec)');
ylabel('position (m)');
grid on;
% Dynamic and control loop
while (t < 10)
    
    % Update Time
    t        = t + dt;      
    
    % Update Desire    
    if t < 0.1
        xDes = [0 ; 0];
    else
        xDes = [1 ; 0];
    end;
  
    % Compute Control
    e = xDes(1) - x(1);
    ie = ie + e;
    de = (e  - pe) / dt;
    pe = e;    
    u =  Kp * e + Ki * ie + Kd * de;
 
    % Update Actual

    x = A * x + B * (u - 5);
    
    % Update error
    err = norm(xDes(1) - x(1));
     
    % Plot
    figure(h);
    hold on;
    plot(t, xDes(1),'b.', 'MarkerSize',10);
    plot(t, x(1)   ,'r.', 'MarkerSize',10);  
    hold off;
    drawnow;
    err
end;
 