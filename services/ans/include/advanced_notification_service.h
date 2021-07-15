/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BASE_NOTIFICATION_ANS_STANDARD_SERVICES_ANS_INCLUDE_ADVANCED_NOTIFICATION_SERVICE_H
#define BASE_NOTIFICATION_ANS_STANDARD_SERVICES_ANS_INCLUDE_ADVANCED_NOTIFICATION_SERVICE_H

#include <list>
#include <memory>
#include <mutex>

#include "event_handler.h"
#include "event_runner.h"
#include "refbase.h"

#include "ans_manager_stub.h"
#include "notification.h"
#include "notification_sorting_map.h"

namespace OHOS {
namespace Notification {

class AdvancedNotificationService final : public AnsManagerStub {
public:
    ~AdvancedNotificationService() override;
    DISALLOW_COPY_AND_MOVE(AdvancedNotificationService);

    static sptr<AdvancedNotificationService> GetInstance();

    ErrCode Publish(const std::string &label, const sptr<NotificationRequest> &request) override;
    ErrCode Cancel(int notificationId, const std::string &label) override;
    ErrCode CancelAll() override;
    ErrCode AddSlots(const std::vector<sptr<NotificationSlot>> &slots) override;
    ErrCode RemoveSlotByType(const NotificationConstant::SlotType slotType) override;
    ErrCode AddSlotGroups(std::vector<sptr<NotificationSlotGroup>> groups) override;
    ErrCode GetSlotByType(const NotificationConstant::SlotType slotType, sptr<NotificationSlot> &slot) override;
    ErrCode GetSlots(std::vector<sptr<NotificationSlot>> &slots) override;
    ErrCode GetSlotGroup(const std::string &groupId, sptr<NotificationSlotGroup> &group) override;
    ErrCode GetSlotGroups(std::vector<sptr<NotificationSlotGroup>> &groups) override;
    ErrCode RemoveSlotGroups(const std::vector<std::string> &groupIds) override;
    ErrCode GetActiveNotifications(std::vector<sptr<NotificationRequest>> &notifications) override;
    ErrCode GetActiveNotificationNums(int &num) override;
    ErrCode GetAllActiveNotifications(std::vector<sptr<Notification>> &notifications) override;
    ErrCode GetSpecialActiveNotifications(
        const std::vector<std::string> &key, std::vector<sptr<Notification>> &notifications) override;
    ErrCode SetNotificationAgent(const std::string &agent) override;
    ErrCode GetNotificationAgent(std::string &agent) override;
    ErrCode CanPublishAsBundle(const std::string &representativeBundle, bool &canPublish) override;
    ErrCode PublishAsBundle(
        const sptr<NotificationRequest> notification, const std::string &representativeBundle) override;
    ErrCode SetNotificationBadgeNum(int num) override;
    ErrCode GetBundleImportance(int &importance) override;
    ErrCode SetDisturbMode(NotificationConstant::DisturbMode mode) override;
    ErrCode GetDisturbMode(NotificationConstant::DisturbMode &mode) override;
    ErrCode HasNotificationPolicyAccessPermission(bool &granted) override;
    ErrCode SetPrivateNotificationsAllowed(bool allow) override;
    ErrCode GetPrivateNotificationsAllowed(bool &allow) override;
    ErrCode Delete(const std::string &key) override;
    ErrCode DeleteByBundle(const std::string &bundle) override;
    ErrCode DeleteAll() override;
    ErrCode GetSlotsByBundle(const std::string &bundle, std::vector<sptr<NotificationSlot>> &slots) override;
    ErrCode UpdateSlots(const std::string &bundle, const std::vector<sptr<NotificationSlot>> &slots) override;
    ErrCode UpdateSlotGroups(
        const std::string &bundle, const std::vector<sptr<NotificationSlotGroup>> &groups) override;
    ErrCode SetNotificationsEnabledForBundle(const std::string &bundle, bool enabled) override;
    ErrCode SetNotificationsEnabledForAllBundles(const std::string &deviceId, bool enabled) override;
    ErrCode SetNotificationsEnabledForSpecialBundle(
        const std::string &deviceId, const std::string &bundleName, bool enabled) override;
    ErrCode SetShowBadgeEnabledForBundle(const std::string &bundle, bool enabled) override;
    ErrCode GetShowBadgeEnabledForBundle(const std::string &bundle, bool &enabled) override;
    ErrCode Subscribe(const sptr<IAnsSubscriber> &subscriber, const sptr<NotificationSubscribeInfo> &info) override;
    ErrCode Unsubscribe(const sptr<IAnsSubscriber> &subscriber, const sptr<NotificationSubscribeInfo> &info) override;
    ErrCode IsAllowedNotify(bool &allowed) override;
    ErrCode IsSpecialBundleAllowedNotify(const std::string &bundle, bool &allowed) override;

private:
    AdvancedNotificationService();

    void StartFilters();
    void StopFilters();
    ErrCode Filter(const sptr<Notification> &notification);

    void AddToNotificationList(const sptr<Notification> &notification);
    void UpdateInNotificationList(const sptr<Notification> &notification);
    ErrCode RemoveFromNotificationList(
        const std::string &bundle, const std::string &label, int notificationId, sptr<Notification> &notification);
    ErrCode RemoveFromNotificationList(const std::string &key, sptr<Notification> &notification);
    std::vector<std::string> GetRemovableNotificationKeys(const std::string &bundle);
    bool IsNotificationExists(const std::string &key);
    void SortNotificationList();
    static bool NotificationCompare(const sptr<Notification> &first, const sptr<Notification> &second);

    sptr<NotificationSortingMap> GenerateSortingMap();

    std::string GetClientBundleName();
    bool IsSystemApp();

private:
    static sptr<AdvancedNotificationService> instance_;
    static std::mutex instanceMutex_;

    std::shared_ptr<OHOS::AppExecFwk::EventRunner> runner_ = nullptr;
    std::shared_ptr<OHOS::AppExecFwk::EventHandler> handler_ = nullptr;
    std::list<sptr<Notification>> notificationList_;
};

}  // namespace Notification
}  // namespace OHOS

#endif  // ADVANCED_NOTIFICATION_SERVICE_H