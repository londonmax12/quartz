$$
\begin{align}
[\text{Term}] &\to 
    \begin{cases}
        \text{IntLit}\\
        \text{Identifier}\\
        (\text{[Expr]})
    \end{cases}
\\
[\text{Expr}] &\to 
    \begin{cases}
        \text{[Term]}\\
        \text{[BinExpr]}
    \end{cases}
\\
[\text{BinExpr}] &\to 
    \begin{cases}
        \text{[Expr]*[Expr]} & \text{Precedence} = 1\\
        \text{[Expr]/[Expr]} & \text{Precedence} = 1\\
        \text{[Expr]+[Expr]} & \text{Precedence} = 0\\
        \text{[Expr]-[Expr]} & \text{Precedence} = 0
    \end{cases}
\\
[\text{VarType}] &\to 
    \begin{cases}
        int
    \end{cases}
\\
[\text{VarDecl}] &\to var\space \text{Identifier}:\space \text{VarType} = [\text{Expr}];
\\
[\text{Exit}] &\to exit([\text{Expr}]);
\\
[\text{Scope}] &\to \{[\text{Statement}^*]\}
\\
[\text{Statement}] &\to
    \begin{cases}
        [\text{Exit}]\\
        [\text{VarDecl}]\\
        [\text{Scope}]\\
        if([\text{Expr}]) [\text{Statement}]
    \end{cases}
\\
[\text{Program}] &\to [\text{Statement}]^+
\end{align}
$$