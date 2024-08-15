using Pkg

try 
    @eval import Plots
catch
    Pkg.add("Plots")
end

using Plots

iteracoes = 100000
LR = 0.0000000001

entradas = []
saidas = [] 
resultados = []

#declara os parâmetros com dois valores aleatórios iniciais
parametros = [rand(Float64), rand(Float64)]

function resultado(entradas, slope, offset)
    offset + slope * entradas
end

#Derivada do peso
function slopeDescent(entradas, saida, parametros)
    soma = 0
    for i in eachindex(entradas)
        soma += 2*entradas[i]*(resultado(entradas[i], parametros[2], parametros[1]) - saida[i])
    end
    return soma
end

#derivada do desvio
function offsetDescent(entradas, saida, parametros)
    soma = 0
    for i in eachindex(entradas)
        soma += 2*(resultado(entradas[i], parametros[2], parametros[1]) - saida[i])
    end
    return soma
end



function gradientDescent!(parametros, entradas , saida)
    parametros[2] -= LR*slopeDescent(entradas, saida, parametros)
    parametros[1] -= LR*offsetDescent(entradas, saida, parametros)
end

entradas = [rand(1:1000),rand(1:1000),rand(1:1000),rand(1:1000),rand(1:1000)]
saidas = [rand(1:1000),rand(1:1000),rand(1:1000),-rand(1:1000),rand(1:1000)]
teste = [-1, 10000]
plot(entradas,saidas, seriestype=:"scatter")

resize!(resultados, length(teste))

for i in 1:iteracoes
    
    for j in eachindex(teste)
        resultados[j] = resultado(teste[j], parametros[2], parametros[1]);
    end

    gradientDescent!(parametros, entradas, saidas)
    println(parametros," ",resultados)


    if((i%1000==0)||(i==1))
        plot(entradas,saidas, seriestype=:"scatter")
        plot!(teste,resultados)  
        gui()
        sleep(1)
    end
    
end

gui()
readline()


