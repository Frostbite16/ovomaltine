using Plots

# Calculate the fibonacci sequence but uses saved data from previous run to speed up the process
function fibonacci(SavedTables,n)
    if(n<1)
        return 0
    end
    if(n<2)
        return 1
    end
    if(isassigned(SavedTables, n)&&(isassigned(SavedTables, n-1)))
        return SavedTables[n] + SavedTables[n-1]
    end
    return fibonacci(SavedTables,n-1) + fibonacci(SavedTables,n-2)
end

TimeArray = []
numberArray = []
SavedTables = []
fibonacci(SavedTables,0)

for n = 0:150
    t = @elapsed fib = fibonacci(SavedTables,n)
    resize!(SavedTables,n+1)
    SavedTables[n+1] = fib

    println(n , ":" , fib)
    println(t)

    push!(TimeArray, t)
    push!(numberArray, n)
    
end

display(plot!(numberArray, TimeArray))
display(plot!(numberArray, TimeArray))
readline()

