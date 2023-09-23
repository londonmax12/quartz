$$
\begin{align}
[\text{expr}] &\to \text{int\_lit} | \text{identifier}
\\
[\text{var\_type}] &\to 
    \begin{cases}
        \text{int}
    \end{cases}
\\
[\text{var\_decl}] &\to \text{var}\space \text{identifier}:\space \text{var\_type} = [\text{expr}];
\\
[\text{exit}] &\to exit([\text{expr}]);
\\
[\text{stmt}] &\to
    \begin{cases}
        [\text{exit}]\\
        [\text{var\_decl}]
    \end{cases}
\\
[\text{program}] &\to [\text{stmt}]^+
\end{align}
$$