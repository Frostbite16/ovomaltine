using Plots

# Definindo as coordenadas para desenhar o cachorro
x_body = [0, 0.5, 1, 0.5, 0, 0.5, 0]
y_body = [0, 0.5, 0, -0.5, 0, 0.5, 0]

x_head = [0.5, 0.7, 0.9, 0.7, 0.5]
y_head = [0.5, 0.8, 0.5, 0.2, 0.5]

x_ear_left = [0.55, 0.6, 0.65]
y_ear_left = [0.7, 0.9, 0.7]

x_ear_right = [0.75, 0.8, 0.85]
y_ear_right = [0.7, 0.9, 0.7]

x_tail = [0, -0.1, -0.2, -0.3, -0.4, -0.5]
y_tail = [0, 0.1, 0.2, 0.1, 0, -0.1]

# Desenhando as partes do cachorro
plot(x_body, y_body, lw=5, color=:black, legend=false)
plot!(x_head, y_head, lw=5, color=:black)
plot!(x_ear_left, y_ear_left, lw=5, color=:black)
plot!(x_ear_right, y_ear_right, lw=5, color=:black)
plot!(x_tail, y_tail, lw=5, color=:black)

# Ajustando os limites do gráfico
xlims!(-0.5, 1)
ylims!(-0.5, 1)

