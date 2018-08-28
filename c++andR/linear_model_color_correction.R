repos_globale <- "C:\\Users\\fabien\\Desktop\\Etheostoma\\Photos\\JPGTAMRA\\Photos_Detour\\Nouveau dossier\\Google Doc\\BAI"
repos_colorchecker_summary <- paste(repos_globale,"ColorChecker_tab",sep="\\")
repos_color_correction_pred_linear_model <- paste(repos_globale,"pred_color_correction\\linear_model",sep="\\")
path_ref <- "C:\\Users\\fabien\\Desktop\\Etheostoma\\Photos\\JPGTAMRA\\Photos_Detour\\Nouveau dossier\\ref.csv"

setwd(repos_colorchecker_summary)

ref <- read.table(path_ref,sep=";",header = TRUE)
ref_grey <- ref[19:24,]

files <- system2(command="ls",stderr = TRUE,stdout = TRUE)
files

for (i in 1:length(files)){
 data <- read.table(files[i],sep=";",header = TRUE)
 data_grey <- data[19:24,]
 
 coeffBGR <- matrix(0,2,3)
 coeffBGR[,1] <- lm(ref_grey[,1]~data_grey[,1])$coefficient
 coeffBGR[,2] <- lm(ref_grey[,2]~data_grey[,2])$coefficient
 coeffBGR[,3] <- lm(ref_grey[,3]~data_grey[,3])$coefficient
 colnames(coeffBGR) <- c("B","G","R")
 write.table(coeffBGR,paste(repos_color_correction_pred_linear_model,files[i],sep="\\"),sep=";",row.names = FALSE)
 
 # plot(data_grey[,1],ref_grey[,1])
 # lines(data_grey[,1],data_grey[,1]*coeffBGR[2,1]+coeffBGR[1,1])
 # plot(data_grey[,2],ref_grey[,2])
 # lines(data_grey[,2],data_grey[,2]*coeffBGR[2,2]+coeffBGR[1,2])
 # plot(data_grey[,3],ref_grey[,3])
 # lines(data_grey[,3],data_grey[,3]*coeffBGR[2,3]+coeffBGR[1,3])
 
}





