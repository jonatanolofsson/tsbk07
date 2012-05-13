
LDZ = 50;
Zlim = [LDZ-1,LDZ-1];

%% Create projection matrix
near = 1; far = 80;
right = 0.5; left = -0.5; top = 0.5; bottom = -0.5;
projectionMatrix = [ 2.0*near/(right-left), 0.0, (right+left)/(right-left), 0.0;
                0.0, 2.0*near/(top-bottom), (top+bottom)/(top-bottom), 0.0;
                0.0, 0.0, -(far + near)/(far - near), -2*far*near/(far - near);
                0.0, 0.0, -1.0, 0.0
                ];

%% Create camera matrix
pos = [0;5;15];
lookAt = [10;3;10];
up = [0;1;0];
n = normalize(pos-lookAt);
u = normalize(cross(up,n));
v = cross(n,u);
camera = [
    u' 0;
    v' 0;
    n' 0;
    0 0 0 1
    ];
camera(1:3, 4) = -camera(1:3, 1:3)*pos;

%% Make X
x = 0 : Zlim(1);
n = 2; m = length(x);
X = cell(1, n);
[X{:}] = ndgrid(x);
X = X(end : -1 : 1); 
X = cat(n+1, X{:});
X = reshape(X, [m^n, n]);
clear n;

%% Create triangles
N = m;
N_TRIANGLES = 3*2*N^2;
TRIANGLES = zeros(N_TRIANGLES, 1);
for xi = x(1:end-1)
    for yi = x(1:end-1)
        b = 2*3*(yi + xi*(N-1)) + 1;
        TRIANGLES(b) = xi*N + yi;
        TRIANGLES(b+1) = (xi+1)*N + yi;
        TRIANGLES(b+2) = xi*N + yi + 1;
        
        TRIANGLES(b+3) = xi*N + yi + 1;
        TRIANGLES(b+4) = (xi+1)*N + yi;
        TRIANGLES(b+5) = (xi+1)*N + yi + 1;
    end
end
tri = reshape(TRIANGLES, 3, [])';
%% Create frustum coordinates
fF = [
    -1, 1, 1, -1;
    0,0,0,0;
    1, 1, -1, -1;
    1,1,1,1
];
base = projectionMatrix * camera;
% q = fF([1,3,4],:);
% minibase = base([1,3,4],[1,3,4]);

x = zeros(4,4);
ii = 0;
x = base\fF;
while any(abs(x(2,:)) > 1e-3)
    fF(2,:) = fF(2,:) - 2.0*x(2,:);
    x = base\fF;
    ii = ii + 1;
end
x = x./repmat(x(4,:),4,1);


z = base * x;
z = z./repmat(z(4,:),4,1);
disp(z)
%%
figure(1);clf;
hold all;
%% Extract
N_INDICES = 0;
IND = zeros(N_TRIANGLES, 1);
world2map = 2*[1 0 0 0; 0 0 1 0];
Z = world2map * x;

maplim.left = max(0, floor(min(Z(1,:))));
maplim.right = min(Zlim(1), ceil(max(Z(1,:))));
zx = maplim.left : maplim.right-1;

delta = diff(Z(:,[1:4 1]),1,2);

order_offset = (delta(1, 4)>0) + (delta(1, 2)>0);

dx = zeros(4,1);
yx = zeros(4,1);
for i = 1:4
    if abs(delta(1,i)) < 1e-7
        dx(i) = 0;
        yx(i) = 1e16;
        order_offset = 0;
    else
        dx(i) = delta(2,i) / delta(1,i);
        yx(i) = Z(2,i) - dx(i) * Z(1,i) + dx(i) * maplim.left;
    end
end

for ix = zx
    zy = sort(yx);
    cut = [
            max(0, floor(zy(order_offset+1))) ...
            min(Zlim(2), ceil(zy(order_offset+2)))];
    N_add = 2*(cut(2) - cut(1));
    IND(N_INDICES+1:N_INDICES+N_add) = (2*cut(1):2*cut(2)-1) + 2*ix*(N-1); % N-1 triangles..
    N_INDICES = N_INDICES + N_add;
    plot(ones(4,1)*ix, yx', 'ro')
    yx = yx + dx;
end


%%
scatter(X(:,1), X(:,2), 1e-5);
for i = 1:N_INDICES
    k = IND(i);
    b = 3*k;
    triangle = TRIANGLES([1 2 3 1]+b)'+1;
    plot(X(triangle, 1), X(triangle, 2), 'r')
%     triangle = TRIANGLES([4 5 6 4]+b)'+1;
%     plot(X(triangle, 1), X(triangle, 2), 'r')
end

plot(Z(1,[1:4 1]), Z(2,[1:4 1]), 'g');
% axis equal;
axis([-20 80 -20 80])