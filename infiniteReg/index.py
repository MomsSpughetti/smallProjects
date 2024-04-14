from selenium import webdriver
webdriver_path = '/home/abdalla/Downloads/chrome-linux64/chrome'
# Create a new instance of the Chrome web driver

chrome_options = webdriver.ChromeOptions()
driver = webdriver.Chrome(options=chrome_options)
driver.get('https://students.technion.ac.il/local/tregister/cart')

import time
times = 0
while True:
    try:
        time.sleep(2)
        elem = driver.find_element("id","process_cart_item_request")
        elem.click()
        print("click number: ", times)
        times+=1
        time.sleep(20)
    except:
        pass