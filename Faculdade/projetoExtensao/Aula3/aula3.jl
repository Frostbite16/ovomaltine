using CSV, Plots

data = CSV.File("/home/guilherme/github/ovomaltine/Faculdade/projetoExtensao/Aula3/houseprice.csv")

Y = data.price
X = data.size

scatter(X,Y)


theta0 = 0;
theta1 = 0;

f(x) = theta0 .+ theta1*X

scatter(X,Y)
plot!(X,f(X))

n = length(X)

infer = f(X)

LI0 = 0.9
LI1 = 0.00000008 

function cost(X,Y)
    (1/2)*sum((infer(X)-Y)^2)
end

function PDtheta0(X,Y)
    sum((Y-infer))
end

function PDtheta1(X,Y)
    sum((Y-infer).*X)
end
theta0_temp = LI0 * PDtheta0(X,Y)
theta1_temp = LI1 * PDtheta1(X,Y)

theta0 -= theta0_temp
theta1 -= theta1_temp

theta0_temp = LI0 * PDtheta0(X,Y)
theta1_temp = LI1 * PDtheta1(X,Y)

theta0 -= theta0_temp
theta1 -= theta1_temp

theta0_temp = LI0 * PDtheta0(X,Y)
theta1_temp = LI1 * PDtheta1(X,Y)

theta0 -= theta0_temp
theta1 -= theta1_temp

theta0_temp = LI0 * PDtheta0(X,Y)
theta1_temp = LI1 * PDtheta1(X,Y)

theta0 -= theta0_temp
theta1 -= theta1_temp


plot!(X,f(X))