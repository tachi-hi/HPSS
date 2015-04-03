library(ggplot2)

yuko <- function(x){
	y <- 10^(ceiling( log(x) / log(10) ))
	z <- ceiling(1000 * (x / y)) / 1000
	return ( y * z )
}

data <- read.table("result.csv", header = T, sep = ",")
InstAndFilename <- c("")
for(i in 1:length(data[[1]])){
	InstAndFilename[i] <- paste(data$Inst[i], "\n(", tolower(data$filename[i]), ".wav)", sep="")
}

df <- data.frame(
	ind = 1:length(data[[1]]), 
	inst  = InstAndFilename, 
	file = data[[2]],
	IType = data[[3]], 
	SType = data[[4]], 
	value = data[[5]])

g <- ggplot(df, aes(x=reorder(inst,-ind), value) )
g <- g + geom_bar(stat="identity", fill = "#333333", width = 0.3)
g <- g + geom_text(aes(label= yuko(value) ), vjust=-1, size = 5)
g <- g + xlab("Instrument")
g <- g + ylab("Value") # of Normalized Smoothness Functions")
if(1){
	g <- g + coord_flip()
	g <- g + facet_grid(. ~ SType)
}else{
	g <- g + facet_grid(SType ~ .)
}

g <- g + theme_bw(base_size = 25)
	g <- g + theme(strip.text.x = element_text(size = 48), strip.background = element_rect(colour = "white", fill = "#CCCCCC"))

postscript("smoothness.eps", horizontal = FALSE, onefile = FALSE, paper = "special", height = 14, width = 12)
print(g)
dev.off()


