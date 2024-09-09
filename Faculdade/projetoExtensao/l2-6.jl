using Plots

x = 0:10
y = -5:5
f(x, y) = 0.5x^2-1.2y^2

plot(x, y, f.(x, y), xlabel = "X", ylabel = "Y", zlabel = "f(x, y)", title = "Função f(x, y)", color = :red, linestyle = :solid)