//
// Created by hamac on 2020-05-25.
//

#include "PageTable.h"

int PageTable::_AID = 1;

PageTable::PageTable(int vmemSize, int pageSize) {
    initPageTable(vmemSize, pageSize);
}

void PageTable::initPageTable(int vmemSize, int pageSize)
/**
 * 페이지 테이블을 초기화
 * @param vmemSize
 * @param pageSize
 */
{
    num_pages = vmemSize / pageSize;

    pages = new int[num_pages];
    valid_bit = new bool[num_pages];

    fill(pages, pages + num_pages, -1);
    fill(valid_bit, valid_bit + num_pages, false);
}

int PageTable::allocate(int target_size)
/**
 * 페이지 테이블에 페이지 할당
 * @param target_size
 * @return
 */
{
    int AID = getNewAID();
    pageNum[AID] = target_size;

    int start = 0;
    bool c = false;

    for (int i = 0; i < num_pages; i++) {
        if (pages[i] == -1 and !c) {
            c = true;
            start = i;
        }
        if (pages[i] == -1 and c) {
            if (i - start + 1 == target_size) {
                for (int j = start; j < start + target_size; j++) {
                    pages[j] = AID;
                }
                break;
            }
        }
        if (pages[i] != -1 and c) {
            c = false;
        }
    }

    return AID;
}

bool PageTable::release(int AID)
/**
 * 페이지 테이블에서 페이지 해제
 * @param AID
 * @return
 */
{
    bool result = false;
    for (int i = 0; i < num_pages; i++) {
        if (pages[i] == AID) {
            pages[i] = -1;
            valid_bit[i] = false;
            result = true;
        }
    }

    return result;
}

void PageTable::setValidBit(int AID)
/**
 * 페이지 테이블에서 해당 AID의 ValidBit true로 만듬
 * @param AID
 */
{
    for (int i = 0; i < num_pages; i++) {
        if (pages[i] == AID) {
            valid_bit[i] = true;
        }
    }
}

void PageTable::setInvalidBit(int AID)
/**
 * 페이지 테이블에서 해당 AID의 ValidBit flase로 만듬
 * @param AID
 */
{
    for (int i = 0; i < num_pages; i++) {
        if (pages[i] == AID) {
            valid_bit[i] = false;
        }
    }
}

vector<int> PageTable::getPages()
/**
 * 페이지 테이블의 할당 내용을 전부 리턴함
 * @return
 */
{
    vector<int> result;
    for (int i = 0; i < num_pages; i++)
        result.push_back(pages[i]);
    return result;
}

vector<int> PageTable::getValidBit()
/**
 * 페이지 테이블의 ValidBit을 전부 리턴함
 * @return
 */
{
    vector<int> result;
    for (int i = 0; i < num_pages; i++) {
        if (pages[i] != -1) {
            result.push_back((valid_bit[i]) ? 1 : 0);
        } else {
            result.push_back(-1);
        }
    }
    return result;
}

int PageTable::getNewAID()
/**
 * 새로운 AID를 하나 생성함
 * @return
 */
{
    return _AID++;
}

int PageTable::getPageNum(int AID)
/**
 * 해당 AID의 페이지 개수를 리턴함
 * @param AID
 * @return
 */
{
    return pageNum[AID];
}

bool PageTable::isValid(int AID)
/**
 * 해당 AID의 페이지의 ValidBit을 리턴함
 * @param AID
 * @return
 */
{
    for (int i = 0; i < num_pages; i++) {
        if (pages[i] == AID) {
            return valid_bit[i];
        }
    }
    return false;
}

bool PageTable::isPageExist(int AID)
/**
 * 해당 AID의 페이지가 존재하는지 리턴
 * @param AID
 * @return
 */
{
    for (int i = 0; i < num_pages; i++) {
        if (pages[i] == AID) {
            return true;
        }
    }
    return false;
}

set<int> PageTable::getAID()
/**
 * 페이지 테이블의 모든 AID를 set으로 리턴
 * @return
 */
{
    set<int> result;
    for (int i = 0; i < num_pages; i++) {
        result.insert(pages[i]);
    }
    return result;
}
