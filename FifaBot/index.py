#from selenium import webdriver
#from selenium.webdriver.chrome.service import Service as ChromeService
#from webdriver_manager.chrome import ChromeDriverManager


from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

#driver = webdriver.Chrome(service=ChromeService(ChromeDriverManager().install()))

options = Options()
#options.add_argument('--headless')
options.add_argument('--no-sandbox')
options.add_argument('--disable-dev-shm-usage')
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)

driver.get('https://www.ea.com/ea-sports-fc/ultimate-team/web-app/')

time.sleep(100)

price = 10000

while 1:
    #change price
    time.sleep(3)

    """
        try:
            elem = driver.find_element(By.XPATH, '//div[@class="search-prices"]/div[3]//input[@class="ut-number-input-control"]')
            print(elem.get_attribute("value"))
            elem.clear()
            elem.send_keys(price)
            print(elem.get_attribute("value"))
            price += 250
        except Exception as e:
            print(e)
            
    """


    #search
    try:
        elemSearch = driver.find_element(By.XPATH, '//button[text()="Search"]')
        elemSearch.click()
    except Exception as e:
        print(e)

    #buy now
    try:
        elementBuy = WebDriverWait(driver, 2).until(
            EC.presence_of_element_located((By.XPATH, '//button[@class="btn-standard buyButton currency-coins"]'))
        )
        elementBuy.click()

        elemOk = WebDriverWait(driver, 2).until(
            EC.presence_of_element_located((By.XPATH, '//span[@class="btn-text"][text()="Ok"]'))
        )
        elemOk.click()
    except Exception as e:
        print(e)

    #return to search page
"""    try:
        elemBack = driver.find_element(By.XPATH, '//button[@class="ut-navigation-button-control"]')
        elemBack.click()
    except Exception as e:
        print(e)"""



#finish
driver.quit()

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


