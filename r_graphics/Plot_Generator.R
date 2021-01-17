library(ggplot2)
library(ggthemes)
######################################################################## The operator +

my_theme = theme(
  line = element_line(colour="black"),
  text = element_text(colour="black"),
  axis.title = element_text(size = 14),
  axis.text = element_text(colour="black", size=12),
  strip.text = element_text(size=12),
  legend.key=element_rect(colour=NA, fill =NA),
  panel.grid = element_blank(),   
  panel.border = element_rect(fill = NA, colour = "black", size=1),
  panel.background = element_rect(fill = "white", colour = "black"), 
  strip.background = element_rect(fill = NA),
  legend.position = c(.3, .95),
  legend.justification = c("right", "top"),
  legend.box.just = "right",
  legend.box.background = element_rect(),
  legend.margin = margin(6, 6, 6, 6),
  legend.box.margin = margin(6, 6, 6, 6),
  legend.text = element_text(size = 13, colour = "black"),
  legend.key.size = unit(2,"line")
)

theam_down = theme(
  line = element_line(colour="black"),
  text = element_text(colour="black"),
  axis.title = element_text(size = 14),
  axis.text = element_text(colour="black", size=12),
  strip.text = element_text(size=12),
  legend.key=element_rect(colour=NA, fill =NA),
  panel.grid = element_blank(),   
  panel.border = element_rect(fill = NA, colour = "black", size=1),
  panel.background = element_rect(fill = "white", colour = "black"), 
  strip.background = element_rect(fill = NA),
  legend.position = c(.98, .3),
  legend.justification = c("right", "top"),
  legend.box.just = "right",
  legend.box.background = element_rect(),
  legend.margin = margin(6, 6, 6, 6),
  legend.box.margin = margin(6, 6, 6, 6),
  legend.text = element_text(size = 13, colour = "black"),
  legend.key.size = unit(2,"line")
)

#################### JavaScript vs Code Generation
test_data <-
data.frame(
JavaScript      =  c(158,440,765,3887 ),
Code_Generation =  c(240,280,418, 1534),
x    = c(10,50,100,500))


ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Function count") 


#################### Scale factor
test_data <-
data.frame(
JavaScript      =  c(45,40,47,78,410,1512),
Code_Generation =  c(111,112,114,134,311,850),
x    = c(100,1000,10000,100000,1000000,10000000))


ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,color = "JavaScript")) + 
geom_line(aes(y = Code_Generation, color = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +xlab("Scale factor") 




#################### Argument coun
test_data <-
data.frame(
JavaScript      =  c(261,321,396,499,686),
Code_Generation =  c(137,142,180,211,256),
x    = c(1,5,10,15,20))


ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Argument count") 


####################### bar chart Function count
test_data <-
data.frame(
values      =  c(17,27,38,67,222,252,380,1467),
Code_Generation =  c("Code Generation","Code Generation","Code Generation","Code Generation","Code Execution","Code Execution","Code Execution","Code Execution"),
x    = c("10","50","100","500"))

test_data$x<-sort(unique(test_data$x))

ggplot(data=test_data, aes(x=x,y=values,fill= Code_Generation)) +
geom_bar(position="stack", stat="identity")+
ylab("Execution Time  in ms") +
xlab("Function count") +
my_theme+theme(legend.title = element_blank())



####################### bar chart Argument count
test_data <-
data.frame(
values      =  c(1.72,2.39,2.365,2.41,2.7,135,139,178,209,253),
Code_Generation =  c("Code Generation","Code Generation","Code Generation","Code Generation","Code Generation","Code Execution","Code Execution","Code Execution","Code Execution","Code Execution"),
x    = c("1","5","10","15","20"))

test_data$x<-sort(unique(test_data$x))

ggplot(data=test_data, aes(x=x,y=values,fill= Code_Generation)) +
geom_bar(position="stack", stat="identity")+
ylab("Execution Time in ms") +
xlab("Argument count") +
my_theme+theme(legend.title = element_blank())



######################################################################## The operator -

#################### JavaScript vs Code Generation
test_data <-
data.frame(
Code_Generation      =  c(85.1,122.44,191.8,760.3),
JavaScript=  c(760,860,1095,1883),
x    = c(10,50,100,500))


ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Function count") 


#################### Scale factor
test_data <-
data.frame(
JavaScript      =  c(49,44,42,80,398,1479),
Code_Generation =  c(34,43,37,60,165,519),
x    = c(100,1000,10000,100000,1000000,10000000))


ggplot(test_data, aes(x,xlab="Scale factor" )) + 
geom_line(aes(y = JavaScript,color = "JavaScript")) + 
geom_line(aes(y = Code_Generation, color = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Scale factor") 



#################### Argument count
test_data <-
data.frame(
JavaScript      =  c(261,320,399,491,670),
Code_Generation =  c(85.7,117.8,146.3,201.53,263.99),
           x    = c(1,5,10,15,20))

ggplot(test_data, aes(x ,xlab="Argument count")) + 
geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Argument count") 


####################### bar chart Function count
test_data <-
data.frame(
values      =  c(2.1,2.44,2.8,7.3,83,120,189,753),
Code_Generation =  c("Code Generation","Code Generation","Code Generation","Code Generation","Code Execution","Code Execution","Code Execution","Code Execution"),
x    = c("10","50","100","500"))

test_data$x<-sort(unique(test_data$x))

ggplot(data=test_data, aes(x=x,y=values,fill= Code_Generation, ylab = "Execution Time" )) +
geom_bar(position="stack", stat="identity")+
ylab("Execution Time in ms") +
xlab("Function count") +
my_theme+theme(legend.title = element_blank())


######################################################################## The operator *

#################### JavaScript vs Code Generation
test_data <-
data.frame(
JavaScript      =  c(2166,2278,2537,3096),
Code_Generation =  c(152.9,280.14,492.5,2165.6),
           x    =  c(10,50,100,500))


ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
  theam_down+
ylab("Execution Time in ms") +
xlab("Function count") 


#################### Scale factor
test_data <-
data.frame(
JavaScript      =  c(51,45,51,87,505,1843),
Code_Generation =  c(49,42,54,80,407,1459),
x    = c(100,1000,10000,100000,1000000,10000000))


ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,color = "JavaScript")) + 
geom_line(aes(y = Code_Generation, color = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Scale factor") 



#################### 
test_data <-
data.frame(
JavaScript      =  c(329,383,483,578,793),
Code_Generation =  c(136,181.5,210,271.3,328.4),
x               =  c(1,5,10,15,20))

ggplot(test_data, aes(x)) + 
geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
my_theme+
ylab("Execution Time in ms") +
xlab("Argument count") 


####################### bar chart Function count
test_data <-
data.frame(
values          =  c(1.9,2.14,2.5,5.6,152.9,280.14,492.5,2165.6),
Code_Generation =  c("Code Generation","Code Generation","Code Generation","Code Generation","Code Execution","Code Execution","Code Execution","Code Execution"),
x               =  c("10","50","100","500"))

test_data$x<-sort(unique(test_data$x))

ggplot(data=test_data, aes(x=x,y=values,fill= Code_Generation)) +
geom_bar(position="stack", stat="identity")+
ylab("Execution Time in ms") +
xlab("Function count") +
my_theme+theme(legend.title = element_blank())


######################################################################## The operator /

#################### JavaScript vs Code Generation
test_data <-
  data.frame(
    JavaScript      =  c(16405,16560,18135,20771),
    Code_Generation =  c(408.9,1706.3,3317.6,16405.7),
    x    = c(10,50,100,500))


ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
  geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
  theam_down+
  ylab("Execution Time in ms") +
  xlab("Function count") 


#################### Scale factor
test_data <-
  data.frame(
    JavaScript      =  c(51,49,67,240,1910,7442),
    Code_Generation =  c(48,42,77.8,316.7,2755,10938),
    x    = c(100,1000,10000,100000,1000000,10000000))


ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = Code_Generation, color = "Code Generation"))+
  my_theme+
  ylab("Execution Time in ms") +
  xlab("Scale factor") 

#################### 
test_data <-
  data.frame(
    JavaScript      =  c(888,937,997,1073,1278),
    Code_Generation =  c(150.8,281,497.3,674.6,887.7),
    x    = c(1,5,10,15,20))


ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,colour = "JavaScript")) + 
  geom_line(aes(y = Code_Generation, colour = "Code Generation"))+
  theam_down+
  ylab("Execution Time in ms") +
  xlab("Argument count") 


####################### bar chart Function count
test_data <-
  data.frame(
    values      =  c(1.9,2.14,2.5,5.6,152.9,280.14,492.5,2165.6),
    Code_Generation =  c("Code Generation","Code Generation","Code Generation","Code Generation","Code Execution","Code Execution","Code Execution","Code Execution"),
    x    = c("10","50","100","500"))

test_data$x<-sort(unique(test_data$x))

ggplot(data=test_data, aes(x=x,y=values,fill= Code_Generation)) +
  geom_bar(position="stack", stat="identity", inherit.aes = TRUE )+ my_theme+
  ylab("Execution Time in ms") +
  xlab("Function count")+theme(legend.title = element_blank())
  



################################################ TPC-H Queries
################################################ TPC-H 3 

tpch3_theme = theme(
  line = element_line(colour="black"),
  text = element_text(colour="black"),
  axis.title = element_text(size = 14),
  axis.text = element_text(colour="black", size=12),
  strip.text = element_text(size=12),
  legend.key=element_rect(colour=NA, fill =NA),
  panel.grid = element_blank(),   
  panel.border = element_rect(fill = NA, colour = "black", size=1),
  panel.background = element_rect(fill = "white", colour = "black"), 
  strip.background = element_rect(fill = NA),
  legend.position = c(.37, .95),
  legend.justification = c("right", "top"),
  legend.box.just = "right",
  legend.box.background = element_rect(),
  legend.margin = margin(6, 6, 6, 6),
  legend.box.margin = margin(6, 6, 6, 6),
  legend.text = element_text(size = 13, colour = "black"),
  legend.key.size = unit(2,"line")
)


test_data <-
  data.frame(
    JavaScript      =  c(18.612,34.485,50.447,66.922,83.037),
    WebAssembly_Bare=  c(0.21,0.42,0.61,0.83,0.99),
    x               =  c(0.20,0.40,0.60,0.80,1.00))


ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = WebAssembly_Bare, color = "WebAssembly (Bare Metal)"))+
  tpch3_theme+
  ylab("Execution Time in sec") +
  xlab("Scale factor") 




test_data <-
  data.frame(
    JavaScript      =  c(18.612,34.485,50.447,66.922,83.037),
    WebAssembly_Brow=  c(1.20,2.30,3.40,4.65,5.57),
    x               =  c(0.20,0.40,0.60,0.80,1.00))

ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = WebAssembly_Brow, color = "WebAssembly (Browser)"))+
  tpch3_theme+
  ylab("Execution Time in sec") +
  xlab("Scale factor") 


test_data <-
  data.frame(
    CL              =  c(0.22,0.43,0.73,0.84,1.18),
    WebAssembly_Brow=  c(1.20,2.30,3.40,4.65,5.57),
    WebAssembly_Bare=  c(0.21,0.42,0.61,0.83,0.99),
    x               =  c(0.20,0.40,0.60,0.80,1.00))


ggplot(test_data, aes(x)) + 
  geom_line(aes(y = CL,color = "C++")) + 
  geom_line(aes(y = WebAssembly_Brow, color = "WebAssembly (Browser)"))+
  geom_line(aes(y = WebAssembly_Bare, color = "WebAssembly (Bare Metal)"))+
  tpch3_theme+
  ylab("Execution Time in sec") +
  xlab("Scale factor") 


######################################################## End TPC-H 3

my_theme2 = theme(
  line = element_line(colour="black"),
  text = element_text(colour="black"),
  axis.title = element_text(size = 14),
  axis.text = element_text(colour="black", size=12),
  strip.text = element_text(size=12),
  legend.key=element_rect(colour=NA, fill =NA),
  panel.grid = element_blank(),   
  panel.border = element_rect(fill = NA, colour = "black", size=1),
  panel.background = element_rect(fill = "white", colour = "black"), 
  strip.background = element_rect(fill = NA),
  legend.position = c(.365, 1),
  legend.justification = c("right", "top"),
  legend.box.just = "right",
  legend.box.background = element_rect(),
  legend.margin = margin(6, 6, 6, 6),
  legend.box.margin = margin(6, 6, 6, 6),
  legend.text = element_text(size = 13, colour = "black"),
  legend.key.size = unit(2,"line")
)

### TPCH-1
test_data <-
  data.frame(
    JavaScript      =  c(0.934,1.55196,2.17502,3.1326,3.91509,4.61169,5.254035, 6.208825, 7.0133, 7.977685 ),
    WebAssembly_Brow=  c(0.796,1.318, 2.063, 3.078, 3.432,4.384, 5.292,6.096,6.511,7.42 ),
    WebAssembly_Bare=  c(0.53125,1.1875,2.26562,3.25,4.45312,5.23438,5.95312,6.60938,7.17697, 7.17697  ),
    CL              =  c(0.220281,0.431189,0.668764,0.941662,1.19371,1.45307,1.71907, 2.05089, 2.33284, 2.60614 ),
    x               =  c(0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.00))

ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = WebAssembly_Brow, color = "WebAssembly (Browser)"))+
  geom_line(aes(y = WebAssembly_Bare, color = "WebAssembly (Bare Metal)"))+
  geom_line(aes(y = CL, color = "C++"))+
  my_theme2+
  ylab("Execution Time in sec") +
  xlab("Scale factor") 

############################
### TPCH-2

test_data <-
  data.frame(
    JavaScript      =  c(0.034855, 0.065,0.026,0.046,0.058,0.072,0.074,0.124,0.131,0.133),
    WebAssembly_Brow=  c(0.00399995, 0.00900006,0.0149999,0.0220001, 0.056, 0.0400002, 0.0480001, 0.0570002, 0.0709999, 0.0869999 ),
    WebAssembly_Bare=  c(0.00385094, 0.00614095, 0.010093, 0.013726, 0.018472, 0.026057, 0.0326891, 0.034411, 0.0437322,0.052386),
    CL              =  c(0.0103099,0.0131419, 0.0177319, 0.0202658, 0.0285699, 0.0288239, 0.0314941, 0.0377438, 0.0468471,0.0528409 ),
    x               =  c(0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.00))

ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = WebAssembly_Brow, color = "WebAssembly (Browser)"))+
  geom_line(aes(y = WebAssembly_Bare, color = "WebAssembly (Bare Metal)"))+
  geom_line(aes(y = CL, color = "C++"))+
  my_theme2+
  ylab("Execution Time in sec") +
  xlab("Scale factor")

##########################
### TPCH-4
test_data <-
  data.frame(
    JavaScript      =  c(0.793,1.395,1.941,2.544,3.075,4.864,5.794,5.661,7.071,6.424),
    WebAssembly_Brow=  c(0.126,0.246,0.4,0.479, 0.623, 0.786, 0.896, 1.081, 1.216, 1.286),
    WebAssembly_Bare=  c(0.0726092, 0.137791, 0.210132, 0.272758, 0.352273, 0.419763, 0.490544, 0.582052, 0.668378, 0.709279),
    CL              =  c(0.278871, 0.343714, 0.414767, 0.492495, 0.589163, 0.669161, 0.712487, 0.784284, 0.887799, 0.968208),
    x               =  c(0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.00))

ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = WebAssembly_Brow, color = "WebAssembly (Browser)"))+
  geom_line(aes(y = WebAssembly_Bare, color = "WebAssembly (Bare Metal)"))+
  geom_line(aes(y = CL, color = "C++"))+
  my_theme2+
  ylab("Execution Time in sec") +
  xlab("Scale factor")

#########################
### TPCH-5
test_data <-
  data.frame(
    JavaScript      =  c(0.226,0.371,0.496,0.741,0.841,1.056,1.132,1.271,1.498,1.721),
    WebAssembly_Brow=  c(0.049,0.0999999,0.15,0.221,0.328,0.424,0.496,0.624,0.759,0.895),
    WebAssembly_Bare=  c(0.0306771, 0.066509, 0.102042, 0.195211, 0.213268, 0.275154, 0.35028, 0.423238, 0.527701, 0.60996),
    CL              =  c(0.029943, 0.0605278, 0.105952, 0.139677, 0.21095, 0.263843, 0.324143, 0.371238, 0.484181, 0.534387),
    x               =  c(0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.00))

ggplot(test_data, aes(x)) + 
  geom_line(aes(y = JavaScript,color = "JavaScript")) + 
  geom_line(aes(y = WebAssembly_Brow, color = "WebAssembly (Browser)"))+
  geom_line(aes(y = WebAssembly_Bare, color = "WebAssembly (Bare Metal)"))+
  geom_line(aes(y = CL, color = "C++"))+
  my_theme2+
  ylab("Execution Time in sec") +
  xlab("Scale factor")





