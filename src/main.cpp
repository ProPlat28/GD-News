// GD News - ProPlat
// Click "More Games" (the News button) on the main menu -> 9 news pages,
// navigable with the side arrows and the Left / Right arrow keys.

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/web.hpp>

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace geode::prelude;

// ---------------------------------------------------------------------------
// Page data
// ---------------------------------------------------------------------------
namespace {
    struct Para {
        std::string text;
        bool blue  = false;
        std::string url;
        bool party = false;
        bool eyes  = false;
        std::string displayDomain; // overrides domain shown in the link popup
    };

    struct Page {
        std::string date;
        std::string title;
        std::vector<Para> paras;
        bool bigEyes  = false;
        int  image    = 1;
        std::string videoUrl;         // non-empty -> thumbnail opens YT confirm popup
        std::string imageUrl;         // non-empty (and videoUrl empty) -> thumbnail opens link popup
        std::string thumbDisplayDomain; // overrides domain shown in thumbnail popup
    };

    Para txt(std::string s, bool party = false) {
        Para p; p.text = std::move(s); p.party = party; return p;
    }
    Para lnk(std::string s, std::string url = "", bool eyes = false) {
        Para p; p.text = std::move(s); p.blue = true; p.url = std::move(url); p.eyes = eyes; return p;
    }

    // The counter always reads "X of TOTAL_PAGES" even when fewer pages are active.
    constexpr int    TOTAL_PAGES  = 9;
    constexpr size_t ACTIVE_PAGES = 9; // all pages unlocked

    std::vector<Page> const& pages() {
        static std::vector<Page> const data = [] {
            std::vector<Page> all;

            { // 1
                Page p;
                p.date     = "October 18, 2026";
                p.title    = "New Event Level: Shrimpire State";
                p.image    = 1;
                p.videoUrl = "https://www.youtube.com/watch?v=_B8-o6hU57Y";
                p.paras    = {
                    txt("The new Event level, \"Shrimpire State\" by Debihan and Naevskai is live!", true),
                    txt("Check it out in-game or watch here:"),
                    lnk("\"shrimple\" may or may not be a new wraith code", "", true),
                };
                all.push_back(p);
            }
            { // 2
                Page p;
                p.date     = "September 13, 2026";
                p.title    = "New Event Level: Bedtime";
                p.image    = 2;
                p.videoUrl = "https://www.youtube.com/watch?v=NgHLQeYr-qk";
                p.paras    = {
                    txt("The new Event level, \"Bedtime\" by nasgubb, is live!", true),
                    txt("Check it out in-game or watch here:"),
                };
                all.push_back(p);
            }
            { // 3 — bigEyes, no link, no popup
                Page p;
                p.date    = "September 5, 2026";
                p.bigEyes = true;
                p.image   = 3;
                all.push_back(p);
            }
            { // 4
                Page p;
                p.date     = "July 24, 2026";
                p.title    = "New Event Level: MOAI";
                p.image    = 4;
                p.videoUrl = "https://www.youtube.com/watch?v=9j77dHeQnQ0";
                p.paras    = {
                    txt("The new Event level, \"MOAI\" by SeniorSaturno, is here! Try in-game or watch below. It uses a new song by Exyl, Teminite, and Creo."),
                };
                all.push_back(p);
            }
            { // 5 — no video thumbnail, clickable forms link
                Page p;
                p.date  = "July 20, 2026";
                p.title = "The Geometry Dash Awards 2026 Suggestions";
                p.image = 5;
                p.imageUrl = "https://docs.google.com/forms/d/e/1FAIpQLSfoPW-mB37-zAv7ze6MhGiN8uuGu1nvmafs9BLxae9MTBhDXg/closedform";
                p.thumbDisplayDomain = "forms.gle";
                p.paras = {
                    txt("Now that the first half of 2026 is done, we are taking suggestions for The Geometry Dash Awards 2026, taking place in January 2027. This includes feedback on the Jury system."),
                    []{
                        auto p = lnk("https://forms.gle/wypDbMAt6Ss1webf8",
                            "https://docs.google.com/forms/d/e/1FAIpQLSfoPW-mB37-zAv7ze6MhGiN8uuGu1nvmafs9BLxae9MTBhDXg/closedform");
                        p.displayDomain = "forms.gle";
                        return p;
                    }(),
                };
                all.push_back(p);
            }
            { // 6
                Page p;
                p.date     = "June 22, 2026";
                p.title    = "Geometry Dash Spotlight Channel";
                p.image    = 6;
                p.videoUrl = "https://www.youtube.com/watch?v=Xp_QOfXeuRM";
                p.paras    = {
                    txt("Geometry Dash Spotlight is now live!"),
                    txt("A new channel where we will be sharing Event levels, community highlights, and more."),
                    txt("Check it out :)"),
                };
                all.push_back(p);
            }
            { // 7
                Page p;
                p.date     = "April 6, 2026";
                p.title    = "Duality Gauntlet Contest";
                p.image    = 7;
                p.videoUrl = "https://www.youtube.com/watch?v=nDBhWpen-eU";
                p.paras    = {
                    txt("The Duality Gauntlet Contest is here! Watch the video for more info :)"),
                };
                all.push_back(p);
            }
            { // 8 — no video, two clickable links
                Page p;
                p.date  = "April 4, 2026";
                p.title = "New Website!";
                p.image = 8;
                p.imageUrl = "https://geometrydash.com";
                p.paras = {
                    txt("The new Geometry Dash website is live! Check out the leaderboards, game pages, and try the playable browser demo of Stereo Madness right from the home page."),
                    lnk("Use code \"geometrydash.com\" in the secret vault for a little bonus gift :)"),
                    lnk("Go to GeometryDash.com", "https://geometrydash.com"),
                };
                all.push_back(p);
            }
            { // 9
                Page p;
                p.date     = "February 28, 2026";
                p.title    = "The Geometry Dash Awards 2025: Winners";
                p.image    = 9;
                p.videoUrl = "https://www.youtube.com/watch?v=gU2VMt0UVJ0";
                p.paras    = {
                    txt("The winners of The Geometry Dash Awards 2025 have been decided! Watch the video to see the best that Geometry Dash had to offer in 2025."),
                };
                all.push_back(p);
            }

            all.resize(std::min(all.size(), ACTIVE_PAGES));
            return all;
        }();
        return data;
    }

    // -----------------------------------------------------------------------
    // Layout constants
    // -----------------------------------------------------------------------
    constexpr float POPUP_W    = 420.f;
    constexpr float POPUP_H    = 280.f;
    constexpr float VIEW_W     = 343.f;
    constexpr float VIEW_H     = 234.7f;
    // Scroll area = font.png panel minus 6 px inset on each side (see init()).
    constexpr float SCROLL_W   = VIEW_W - 12.f;
    // How far the TOP edge of the scroll area is pushed up (bottom edge stays put).
    // 0 = original, bigger = text/thumbnail can go higher before being clipped.
    constexpr float SCROLL_EXTRA_TOP = 6.5f;
    constexpr float SCROLL_H   = VIEW_H - 12.f + SCROLL_EXTRA_TOP; // real visible height of the scroll area
    constexpr float TOP_PAD    = 18.2f;           // page top -> date centre
    constexpr float BOTTOM_PAD = 8.f;             // thumbnail bottom -> page end
    // Text margin inside the scroll area. Same value left and right, so the
    // text uses the whole width of the font.png panel and wraps right before
    // its edge (small pad kept so it never touches the border / scrollbar).
    constexpr float PAD_X      = 12.f;
    constexpr float WRAP_W     = SCROLL_W - PAD_X * 2.f;
    constexpr float LINE_H     = 15.6f;
    constexpr float PARA_GAP   = 7.6f;
    constexpr float IMG_W      = 290.f;                  // thumbnail width (307 = full text width, 286.7 = original)
    constexpr float IMG_BORDER = 1.8f;
    constexpr float IMG_X      = (SCROLL_W - IMG_W) / 2.f; // centred in the scroll area

    // Width of the "Open YouTube" / "Open Link" confirm popup (Geode default is ~350).
    constexpr float LINK_POPUP_W = 250.f;
    ccColor3B const BLUE_TXT = {72, 185, 229};

    float fitScale(char const* font, char const* text, float targetW) {
        auto l = CCLabelBMFont::create(text, font);
        float w = l ? l->getContentSize().width : 0.f;
        return w > 0.f ? targetW / w : 1.f;
    }

    std::vector<std::string> wrapText(std::string const& text, float scale, float maxW) {
        auto probe = CCLabelBMFont::create("", "chatFont.fnt");
        std::vector<std::string> lines;
        std::string cur;
        std::istringstream ss(text);
        std::string word;
        while (ss >> word) {
            std::string attempt = cur.empty() ? word : cur + " " + word;
            probe->setString(attempt.c_str());
            if (!cur.empty() && probe->getContentSize().width * scale > maxW) {
                lines.push_back(cur);
                cur = word;
            } else {
                cur = attempt;
            }
        }
        if (!cur.empty()) lines.push_back(cur);
        return lines;
    }

    CCSprite* makeArrow(bool flip) {
        CCSprite* s = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        if (s) s->setFlipX(flip);
        return s;
    }

    template <class T>
    void forwardKey(T* self, enumKeyCodes key) {
        if constexpr (requires { self->T::keyDown(key, 0.0); })
            self->T::keyDown(key, 0.0);
        else if constexpr (requires { self->T::keyDown(key); })
            self->T::keyDown(key);
    }

    // Extract the host/domain from a URL for display in popups.
    // e.g. "https://forms.gle/abc" -> "forms.gle"
    //      "https://www.youtube.com/watch?v=x" -> "youtube.com"
    std::string extractDomain(std::string const& url) {
        // Strip scheme
        size_t start = url.find("://");
        std::string host = (start != std::string::npos) ? url.substr(start + 3) : url;
        // Strip path
        size_t slash = host.find('/');
        if (slash != std::string::npos) host = host.substr(0, slash);
        // Strip port
        size_t colon = host.find(':');
        if (colon != std::string::npos) host = host.substr(0, colon);
        // Remove leading "www."
        if (host.size() > 4 && host.substr(0, 4) == "www.")
            host = host.substr(4);
        return host;
    }

    bool isYouTubeUrl(std::string const& url) {
        std::string d = extractDomain(url);
        return d == "youtube.com" || d == "youtu.be";
    }

    // Show the appropriate confirm dialog for any URL.
    // YouTube  -> title "Open YouTube",  body "Watch this video on <cy>YouTube</c>?"
    // Other    -> title "Open Link",      body "Open <cy>domain</c> in your browser?"
    // displayDomain overrides the domain shown in the popup (optional).
    void showLinkPopup(std::string const& url, std::string const& displayDomain = "") {
        if (isYouTubeUrl(url)) {
            geode::createQuickPopup(
                "Open YouTube",
                "Watch this video on <cy>YouTube</c>?",
                "Cancel", "Open",
                LINK_POPUP_W,
                [url](FLAlertLayer*, bool btn2) {
                    if (btn2) utils::web::openLinkInBrowser(url);
                }
            );
        } else {
            std::string domain = displayDomain.empty() ? extractDomain(url) : displayDomain;
            std::string body   = "Open <cy>" + domain + "</c> in your browser?";
            geode::createQuickPopup(
                "Open Link",
                body,
                "Cancel", "Open",
                LINK_POPUP_W,
                [url](FLAlertLayer*, bool btn2) {
                    if (btn2) utils::web::openLinkInBrowser(url);
                }
            );
        }
    }
} // namespace

// ---------------------------------------------------------------------------
// Main news popup
// ---------------------------------------------------------------------------
class NewsPopup : public Popup {
protected:
    size_t m_page = 0;
    ScrollLayer*   m_scroll   = nullptr;
    CCNode*        m_pageNode = nullptr;
    CCLabelBMFont* m_counter  = nullptr;
    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;
    CCLayerColor* m_barTrack = nullptr;
    CCLayerColor* m_barThumb = nullptr;
    std::vector<std::string> m_links;
    std::vector<std::string> m_linkDomains;
    std::string m_thumbUrl;
    std::string m_thumbDisplayDomain; // URL opened when clicking the thumbnail (video or image link)

    float m_bodyScale    = 0.6f;
    float m_dateScale    = 0.4f;
    float m_counterScale = 0.5f;
    float m_barTop = 0.f;
    float m_barH   = 0.f;

    bool init() override {
        if (!Popup::init(POPUP_W, POPUP_H, "GJ_square01.png")) return false;

        this->setKeyboardEnabled(true);

        // Gold corners — same approach as AboutBEPopup / PopupWithCorners
        auto addGoldCorner = [this](Anchor anchor, float ox, float oy, bool flipX, bool flipY) {
            auto spr = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
            if (!spr) return;
            spr->setFlipX(flipX);
            spr->setFlipY(flipY);
            m_mainLayer->addChildAtPosition(spr, anchor, ccp(ox, oy));
        };
        addGoldCorner(Anchor::BottomLeft,  24.75f,  24.75f, false, false);
        addGoldCorner(Anchor::BottomRight, -24.75f,  24.75f, true,  false);
        addGoldCorner(Anchor::TopLeft,      24.75f, -24.75f, false, true);
        addGoldCorner(Anchor::TopRight,    -24.75f, -24.75f, true,  true);

        // Font calibration
        m_bodyScale    = fitScale("chatFont.fnt", "The new Event level, \"Shrimpire State\" by Debihan and", 257.f);
        m_dateScale    = fitScale("goldFont.fnt", "October 18, 2026", 99.7f);
        m_counterScale = fitScale("goldFont.fnt", "1 of 9", 39.f);
        float newsScale = fitScale("goldFont.fnt", "News", 55.f);

        // Header
        auto header = CCLabelBMFont::create("News", "goldFont.fnt");
        header->setScale(newsScale);
        m_mainLayer->addChildAtPosition(header, Anchor::Center, {0.f, 120.5f});

        // Counter — always shows "X of 9"
        m_counter = CCLabelBMFont::create("1 of 9", "goldFont.fnt");
        m_counter->setScale(m_counterScale);
        m_counter->setAnchorPoint({1.f, 0.5f});
        m_mainLayer->addChildAtPosition(m_counter, Anchor::Center, {168.f, 114.2f});

        // Inset content panel — font.png is a solid rounded-corner fill with a
        // thin border baked in. One sprite, full size, no sharp corners.
        {
            auto insetPanel = CCSprite::create(
                Mod::get()->expandSpriteName("font.png").c_str()
            );
            if (insetPanel) {
                auto sz = insetPanel->getContentSize();
                insetPanel->setScaleX(VIEW_W / sz.width);
                insetPanel->setScaleY(VIEW_H / sz.height);
                insetPanel->setAnchorPoint({0.f, 0.f});
                insetPanel->ignoreAnchorPointForPosition(false);
                insetPanel->setZOrder(1);
                m_mainLayer->addChildAtPosition(insetPanel, Anchor::Center,
                    {-POPUP_W / 2.f + 38.f, -POPUP_H / 2.f + 11.f});
            }
        }

        // Scroll area — wrapped in a clipping node so content never overflows
        // outside the panel bounds. Inset by INSET on all sides to stay clear
        // of the rounded border.
        constexpr float INSET = 6.f;
        m_scroll = ScrollLayer::create(CCSize{SCROLL_W, SCROLL_H});
        m_scroll->setAnchorPoint({0.f, 0.f});
        m_scroll->ignoreAnchorPointForPosition(false);
        m_scroll->setStealingTouches(true);

        auto stencil = CCSprite::create(
            Mod::get()->expandSpriteName("font.png").c_str()
        );
        if (stencil) {
            auto sz = stencil->getContentSize();
            stencil->setScaleX((VIEW_W - INSET * 2.f) / sz.width);
            stencil->setScaleY(SCROLL_H / sz.height);
            stencil->setAnchorPoint({0.f, 0.f});
            stencil->setPosition({0.f, 0.f});
        }

        auto clipNode = CCClippingNode::create(stencil);
        clipNode->setContentSize({SCROLL_W, SCROLL_H});
        clipNode->setAnchorPoint({0.f, 0.f});
        clipNode->setAlphaThreshold(0.05f);
        clipNode->addChild(m_scroll);
        clipNode->setZOrder(2);
        m_mainLayer->addChildAtPosition(clipNode, Anchor::Center,
            {-POPUP_W / 2.f + 38.f + INSET, -POPUP_H / 2.f + 13.f});

        // Scrollbar
        m_barH   = VIEW_H - 16.f;
        m_barTop = -POPUP_H / 2.f + 11.f + VIEW_H - 8.f;
        // zOrder 3: must be ABOVE the font.png panel (z 1) and the clip node (z 2),
        // otherwise the bar is drawn underneath them and is invisible.
        m_barTrack = CCLayerColor::create(ccc4(190, 130, 100, 70), 2.8f, m_barH);
        m_barTrack->setZOrder(3);
        m_mainLayer->addChildAtPosition(m_barTrack, Anchor::Center,
            {166.f - 1.4f, m_barTop - m_barH});
        m_barThumb = CCLayerColor::create(ccc4(215, 170, 145, 210), 2.8f, 13.f);
        m_barThumb->setZOrder(3);
        m_mainLayer->addChildAtPosition(m_barThumb, Anchor::Center,
            {166.f - 1.4f, m_barTop - 30.f});

        // Side arrows
        auto prevSpr = makeArrow(false);
        auto nextSpr = makeArrow(true);
        if (prevSpr && nextSpr) {
            m_prevBtn = CCMenuItemSpriteExtra::create(
                prevSpr, this, menu_selector(NewsPopup::onPrev));
            m_nextBtn = CCMenuItemSpriteExtra::create(
                nextSpr, this, menu_selector(NewsPopup::onNext));
            m_buttonMenu->addChildAtPosition(
                m_prevBtn, Anchor::Center, {-POPUP_W / 2.f - 28.f, 0.f});
            m_buttonMenu->addChildAtPosition(
                m_nextBtn, Anchor::Center, { POPUP_W / 2.f + 28.f, 0.f});
        }

        this->schedule(schedule_selector(NewsPopup::updateBar));
        this->showPage();
        return true;
    }

    // -----------------------------------------------------------------------
    void showPage() {
        auto const& pg = pages()[m_page];
        m_thumbUrl          = !pg.videoUrl.empty() ? pg.videoUrl : pg.imageUrl;
        m_thumbDisplayDomain = pg.thumbDisplayDomain;
        auto content   = m_scroll->m_contentLayer;

        if (m_pageNode) m_pageNode->removeFromParent();
        m_pageNode = nullptr;
        m_links.clear();
        m_linkDomains.clear();

        auto pageNode = CCNode::create();
        pageNode->setAnchorPoint({0.f, 0.f});
        auto inner = CCNode::create();
        inner->setAnchorPoint({0.f, 0.f});
        pageNode->addChild(inner);

        auto menu = CCMenu::create();
        menu->setPosition({0.f, 0.f});
        inner->addChild(menu, 5);

        // Date
        float cur = TOP_PAD;
        auto date = CCLabelBMFont::create(pg.date.c_str(), "goldFont.fnt");
        date->setScale(m_dateScale);
        date->setAnchorPoint({0.f, 0.5f});
        date->setPosition({PAD_X, -cur});
        inner->addChild(date);

        float imageTop = 0.f;

        if (pg.bigEyes) {
            auto eyes = CCSprite::create("emoji_eyes.png"_spr);
            if (eyes) {
                eyes->setScale(26.f / eyes->getContentSize().width);
                eyes->setAnchorPoint({0.f, 0.5f});
                eyes->setPosition({PAD_X, -(cur + 26.f)});
                inner->addChild(eyes);
            }
            imageTop = 66.6f;
        } else {
            float lineY = cur + 25.f + 17.f;
            if (!pg.title.empty()) {
                cur += 25.f;
                auto title = CCLabelBMFont::create(pg.title.c_str(), "bigFont.fnt");
                float w1 = title->getContentSize().width;
                float s  = std::min(0.52f, w1 > 0.f ? 268.f / w1 : 0.52f);
                title->setScale(s);
                title->setAnchorPoint({0.f, 0.5f});
                title->setPosition({PAD_X, -cur});
                inner->addChild(title);
                lineY = cur + 24.5f;
            }

            float lastCenter = lineY;
            for (auto const& p : pg.paras) {
                // Reserve room for the trailing emoji so it never leaves the panel.
                float paraW = WRAP_W - ((p.party || p.eyes) ? 20.f : 0.f);
                auto lines = wrapText(p.text, m_bodyScale, paraW);
                for (size_t i = 0; i < lines.size(); ++i) {
                    auto lbl = CCLabelBMFont::create(lines[i].c_str(), "chatFont.fnt");
                    if (p.blue) lbl->setColor(BLUE_TXT);
                    float lw = lbl->getContentSize().width * m_bodyScale;
                    bool clickable = !p.url.empty() && lines.size() == 1;

                    if (clickable) {
                        auto item = CCMenuItemSpriteExtra::create(
                            lbl, this, menu_selector(NewsPopup::onLink));
                        item->setScale(m_bodyScale);
                        item->m_baseScale = m_bodyScale;
                        item->m_scaleMultiplier = 1.f; // no shrink on press
                        item->setTag(static_cast<int>(m_links.size()));
                        m_links.push_back(p.url);
                        m_linkDomains.push_back(p.displayDomain);
                        item->setPosition({PAD_X + lw / 2.f, -lineY});
                        menu->addChild(item);
                    } else {
                        lbl->setScale(m_bodyScale);
                        lbl->setAnchorPoint({0.f, 0.5f});
                        lbl->setPosition({PAD_X, -lineY});
                        inner->addChild(lbl);
                    }

                    if (i + 1 == lines.size() && (p.party || p.eyes)) {
                        auto emo = CCSprite::create(
                            p.party ? "emoji_party.png"_spr : "emoji_eyes.png"_spr);
                        if (emo) {
                            float ew = p.party ? 14.5f : 15.6f;
                            emo->setScale(ew / emo->getContentSize().width);
                            emo->setAnchorPoint({0.f, 0.5f});
                            emo->setPosition({PAD_X + lw + 4.f, -lineY});
                            inner->addChild(emo);
                        }
                    }

                    lastCenter = lineY;
                    lineY += LINE_H;
                }
                lineY += PARA_GAP;
            }
            imageTop = lastCenter + 17.6f;
        }

        // Thumbnail
        // Only page 1 uses a PNG (news1.png); every other page uses news<N>.jpg.
        char const* ext = (pg.image == 1) ? "png" : "jpg";
        std::string imgKey = Mod::get()->expandSpriteName(
            fmt::format("news{}.{}", pg.image, ext).c_str());

        float imgH = IMG_W * 9.f / 16.f;
        auto spr = CCSprite::create(imgKey.c_str());
        if (spr) {
            float sc = IMG_W / spr->getContentSize().width;
            imgH     = spr->getContentSize().height * sc;
            spr->setScale(sc);
        }

        if (spr) {
            spr->setAnchorPoint({0.f, 0.f});
            spr->setPosition({IMG_X, -(imageTop + imgH)});
            inner->addChild(spr);
        }

        // If this page has a clickable thumbnail (video or link), add invisible hit button.
        if (!m_thumbUrl.empty()) {
            auto hitNode = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            if (hitNode) {
                hitNode->setScaleX(IMG_W / hitNode->getContentSize().width);
                hitNode->setScaleY(imgH / hitNode->getContentSize().height);
                hitNode->setOpacity(0);

                auto thumbBtn = CCMenuItemSpriteExtra::create(
                    hitNode, this, menu_selector(NewsPopup::onThumbnail));
                thumbBtn->setPosition({
                    IMG_X + IMG_W / 2.f,
                    -(imageTop + imgH / 2.f)
                });
                thumbBtn->m_baseScale = 1.f;
                menu->addChild(thumbBtn, 10);
            }
        }

        float H = std::max(imageTop + imgH + BOTTOM_PAD, SCROLL_H);
        inner->setPosition({0.f, H});
        pageNode->setContentSize({VIEW_W, H});
        content->addChild(pageNode);
        content->setContentSize({VIEW_W, H});
        content->setPositionY(SCROLL_H - H);
        m_pageNode = pageNode;

        // Counter always shows "X of 9"
        m_counter->setString(
            fmt::format("{} of {}", m_page + 1, TOTAL_PAGES).c_str());

        if (m_prevBtn) m_prevBtn->setVisible(m_page > 0);
        if (m_nextBtn) m_nextBtn->setVisible(m_page + 1 < pages().size());
        this->updateBar(0.f);
    }

    // -----------------------------------------------------------------------
    void updateBar(float) {
        if (!m_scroll || !m_barThumb || !m_barTrack) return;
        auto content = m_scroll->m_contentLayer;
        float H    = content->getContentSize().height;
        float minY = SCROLL_H - H;
        if (H <= SCROLL_H + 0.5f) {
            m_barThumb->setVisible(false);
            m_barTrack->setVisible(false);
            return;
        }
        m_barThumb->setVisible(true);
        m_barTrack->setVisible(true);
        float thumbH = std::max(16.f, m_barH * SCROLL_H / H);
        float p = (content->getPositionY() - minY) / (0.f - minY);
        p = std::clamp(p, 0.f, 1.f);
        m_barThumb->setContentSize({2.8f, thumbH});
        float topY  = m_barTop - p * (m_barH - thumbH);
        auto centre = m_mainLayer->getContentSize() * 0.5f;
        m_barThumb->setPosition({
            centre.width  + 166.f - 1.4f,
            centre.height + topY - thumbH
        });
    }

    // -----------------------------------------------------------------------
    void goTo(int delta) {
        int n    = static_cast<int>(pages().size());
        int next = std::clamp(static_cast<int>(m_page) + delta, 0, n - 1);
        if (next == static_cast<int>(m_page)) return;
        m_page = static_cast<size_t>(next);
        this->showPage();
    }

    void onPrev(CCObject*)      { this->goTo(-1); }
    void onNext(CCObject*)      { this->goTo(1); }

    // Thumbnail click: open the appropriate popup.
    void onThumbnail(CCObject*) {
        if (!m_thumbUrl.empty()) showLinkPopup(m_thumbUrl, m_thumbDisplayDomain);
    }

    // Text link click: use unified popup (handles both YT and non-YT).
    void onLink(CCObject* sender) {
        auto idx = static_cast<size_t>(static_cast<CCNode*>(sender)->getTag());
        if (idx < m_links.size()) {
            std::string domain = (idx < m_linkDomains.size()) ? m_linkDomains[idx] : "";
            showLinkPopup(m_links[idx], domain);
        }
    }

    bool handleKey(enumKeyCodes key) {
        switch (key) {
            case KEY_Left:  case KEY_A:  this->goTo(-1); return true;
            case KEY_Right: case KEY_D:  this->goTo(1);  return true;
            case KEY_Up: case KEY_Down: {
                auto content = m_scroll->m_contentLayer;
                float minY   = SCROLL_H - content->getContentSize().height;
                if (minY >= 0.f) return true;
                float y = content->getPositionY() + (key == KEY_Down ? 40.f : -40.f);
                content->setPositionY(std::clamp(y, minY, 0.f));
                return true;
            }
            default: return false;
        }
    }

    void keyDown(enumKeyCodes key, double p1) override {
        if (!this->handleKey(key)) Popup::keyDown(key, p1);
    }

public:
    // Appear instantly — no scale-in animation
    void show() override {
        Popup::show();
        m_mainLayer->stopAllActions();
        m_mainLayer->setScale(1.f);
    }

    static NewsPopup* create() {
        auto ret = new NewsPopup();
        if (ret->init()) { ret->autorelease(); return ret; }
        delete ret;
        return nullptr;
    }
};

// ---------------------------------------------------------------------------
// Hook: More Games / News button + button sprite replacement
// ---------------------------------------------------------------------------
class $modify(NewsMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("more-games-menu");
        if (menu) {
            auto sprfeatured = CCSprite::create("GJ_newsBtn_001.png"_spr);

            std::map<std::string, CCSprite*> idsToBtns = {
                { "more-games-button", sprfeatured }
            };

            for (auto pair = idsToBtns.begin(); pair != idsToBtns.end(); pair++) {
                auto id = pair->first.c_str();
                auto superExpertLoaded = ((strcmp("more-games-button", id) == 0) &&
                    (Loader::get()->isModLoaded("xanii.super_expert")));

                if (!menu->getChildByID(id)) continue;
                if (!pair->second) continue;

                auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildByID(id));
                if (!btn) continue;
                auto contentSize = btn->getContentSize();
                auto existingSprite = btn->getChildByType<CCSprite*>(0);

                if (!existingSprite) continue;

                auto scale = existingSprite->getScale();

                // The replacement image is a single file used at every texture
                // quality. Cocos sizes a sprite as pixels / contentScaleFactor
                // (4 = High, 2 = Medium, 1 = Low), so on Medium/Low it came out
                // 2x / 4x too big. Compensate so it matches the High size.
                float qualityFix = CCDirector::sharedDirector()->getContentScaleFactor() / 4.f;
                if (qualityFix <= 0.f) qualityFix = 1.f;
                pair->second->setScale(scale * qualityFix);

                if (!superExpertLoaded) {
                    btn->setNormalImage(pair->second);
                }

                btn->setContentSize(contentSize);
            }
        }

        return true;
    }

    void onMoreGames(CCObject*) {
        if (auto popup = NewsPopup::create()) popup->show();
    }
};
