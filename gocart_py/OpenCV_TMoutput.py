import cv2
import tensorflow.keras
import numpy as np
import serial
import keyboard
from PIL import Image, ImageOps

# 카메라 캡쳐 객체, 0=내장 카메라
capture = cv2.VideoCapture(1)

# 캡쳐 프레임 사이즈 조절
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

port = 'COM3'
ser = serial.Serial(port, baudrate = 9600)                    #시리얼 포트 설정

## 학습된 모델 불러오기
model_filename = 'keras_model.h5'
model = tensorflow.keras.models.load_model(model_filename, compile = False)

v = 0
num = 10
dir_list = [0, 0, 0, 0]
data = np.ndarray(shape=(1, 224, 224, 3), dtype=np.float32)

# 이미지 전처리
def image_crop(frame):
    height = frame.shape[0]                     #높이, 너비
    width = frame.shape[1]
    x = height//2 - 112                         #시작x, y 설정
    y = width//2 - 112
    frame = frame[x:x+224, y:y+224]    #Crop
    return frame

def preprocessing(frame): 
    size = (224, 224)
    frame_resized = cv2.resize(frame, size, interpolation=cv2.INTER_AREA)
    # 이미지 정규화
    frame_normalized = (frame_resized.astype(np.float32) / 127.0) - 1    
    # 이미지 차원 재조정 - 예측을 위해 reshape 해줍니다.
    frame_reshaped = frame_normalized.reshape((1, 224, 224, 3))
    return frame_reshaped

while True: # 특정 키를 누를 때까지 무한 반복
    # 한 프레임씩 읽기
    ret, frame = capture.read()
    if ret == True:
        # 이미지 자르기 및 출력
        frame = image_crop(frame)
        cv2.imshow("VideoFrame", frame)
        #이미지 전처리
        preprocessed = preprocessing(frame)
        
        # 예측
        prediction = model.predict(preprocessed)
        #print(prediction) # [[0.00533728 0.99466264]]

        # 예측 결과 중 가장 높은 결과 선택
        max_index = np.argmax(prediction[0])
        result = 'S'
        arr = ['W', 'S', 'A', 'D']
        result = arr[max_index]

        #num개씩 받는 부분
        if v == num:
            serval = arr[dir_list.index(max(dir_list))]
            serval = serval.encode()
            #인코딩 방식 새로 추가
            serval = (str(arr[max_index])+'\n').encode("utf-8")

            # 출력
            ser.write(serval)                                     #시리얼 출력
            print(serval, ': ', prediction[0, max_index]*100, '%')
        else:
            for i in range(4):              #Ilovepython
                if result == arr[i]:
                    dir_list[i] += 1

        # 1ms동안 사용자가 키를 누르기를 기다림
        if cv2.waitKey(1) > 0: 
            break 

    else:   
        print("Wait...")

    v += 1
    if v > num:
        v = 0            
# 카메라 객체 반환
capture.release()
 
# 화면에 나타난 윈도우들을 종료
cv2.destroyAllWindows()
