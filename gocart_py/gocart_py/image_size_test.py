import cv2

# 카메라 캡쳐 객체, 0=내장 카메라
capture = cv2.VideoCapture(1)

# 캡쳐 프레임 사이즈 조절
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

def preprocessing(frame):
    height = frame.shape[0]
    width = frame.shape[1]
    x = height//2 - 112
    y = width//2 - 112
    frame_reshaped = frame[x:x+224, y:y+224]
    return frame_reshaped

while True: # 특정 키를 누를 때까지 무한 반복
    # 한 프레임씩 읽기
    ret, frame = capture.read()
    
    frame = preprocessing(frame)
    # 읽어들인 프레임을 윈도우창에 출력
    cv2.imshow("VideoFrame", frame)
    print(frame.shape)
    
    # 1ms동안 사용자가 키를 누르기를 기다림
    if cv2.waitKey(1) > 0: 
        break 
        
# 카메라 객체 반환
capture.release()
 
# 화면에 나타난 윈도우들을 종료
cv2.destroyAllWindows()