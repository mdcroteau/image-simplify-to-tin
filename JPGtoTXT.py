#run using JES

#main method converts a .jpg file to a .txt file
def main():
  #open data and image files
  dataFile = open(pickAFile(), "w")
  image = makePicture(pickAFile())
  
  #get height and width of image
  width = getWidth(image)
  height = getHeight(image)
  dataFile.write("ncols\t" + str(width) + "\n")
  dataFile.write("nrows\t" + str(height) + "\n")
  
  #find RGB values for each pixel
  for x in range(0, width - 1):
    for y in range(0, height -1):
      pixel = getPixel(image, x, y)
      pixelR = getRed(pixel)
      pixelG = getGreen(pixel)
      pixelB = getBlue(pixel)
      dataFile.write(str(pixelR) + " " + str(pixelG) + " " + str(pixelB) + " ")
    dataFile.write("\n")

  #close data file
  dataFile.close()