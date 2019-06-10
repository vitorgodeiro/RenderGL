import cv2

img = cv2.imread('mandrill_256.jpg')
lower_reso = cv2.pyrDown(img)
cv2.imwrite('mandrill_128.jpg', lower_reso)

lower_reso = cv2.pyrDown(lower_reso)
cv2.imwrite('mandrill_64.jpg', lower_reso)

lower_reso = cv2.pyrDown(lower_reso)
cv2.imwrite('mandrill_32.jpg', lower_reso)

lower_reso = cv2.pyrDown(lower_reso)
cv2.imwrite('mandrill_16.jpg', lower_reso)

lower_reso = cv2.pyrDown(lower_reso)
cv2.imwrite('mandrill_8.jpg', lower_reso)

lower_reso = cv2.pyrDown(lower_reso)
cv2.imwrite('mandrill_4.jpg', lower_reso)

lower_reso = cv2.pyrDown(lower_reso)
cv2.imwrite('mandrill_2.jpg', lower_reso)
