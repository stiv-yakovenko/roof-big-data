import cv2,random,numpy,time
import os
cnt=500
src={}
red = numpy.array([36,28,237])
W=256
for r, d, f in os.walk("./full/"):
    for file in f:
       src[file]=os.path.join(r, file)
while True:
    for filename in os.listdir("./labeled"):
        start = time.time()
        img = cv2.imread("./labeled/" + filename)
        end = time.time()
        print (end - start)
        w = img.shape[1]
        h= img.shape[0]
        x0=int(random.uniform(0, w - W))
        y0 = int(random.uniform(0, h - W))
        x1=x0+W
        y1=y0+W
        prt=img[y0:y1,x0:x1]
        # prt0 = img0[]
        cv2.imshow("",prt)
        cv2.waitKey(1)
        numred=0
        colors, counts = np.unique(image.reshape(-1, 3),
                                   return_counts=True,
                                   axis=0)
        start1 = time.time()
        for i in range(0, W):
            for j in range(0, W):
                px=prt[i, j]
                if (numpy.array_equal(px,red)):
                    numred=numred+1
        end1 = time.time()
        print ("t2,",end1 - start1)
        if (numred==0): continue
        print(numred)
        img0 = cv2.imread("./labeled/" + filename)
